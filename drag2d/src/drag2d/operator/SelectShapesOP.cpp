#include "SelectShapesOP.h"

#include "common/visitors.h"
#include "common/Rect.h"
#include "view/MultiShapesImpl.h"
#include "component/AbstractEditCMPT.h"
#include "render/DrawSelectedShapeVisitor.h"
#include "message/SelectShapeSJ.h"
#include "message/SelectShapeSetSJ.h"
#include "message/InsertShapeSJ.h"

namespace d2d
{

SelectShapesOP::SelectShapesOP(wxWindow* wnd, d2d::EditPanelImpl* stage, MultiShapesImpl* shapesImpl,
							   AbstractEditCMPT* callback/* = NULL*/)
	: DrawRectangleOP(wnd, stage)
	, m_callback(callback)
	, m_shapeImpl(shapesImpl)
	, m_bDraggable(true)
{
	m_selection = shapesImpl->GetShapeSelection();
	m_selection->Retain();

	m_first_pos.setInvalid();
}

SelectShapesOP::~SelectShapesOP()
{
	clearClipboard();

 	m_selection->Clear();
 	m_selection->Release();
}

bool SelectShapesOP::OnKeyDown(int keyCode)
{
	if (DrawRectangleOP::OnKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		m_shapeImpl->ClearSelectedShape();
		Clear();
	}
	else if (m_stage->GetKeyState(WXK_CONTROL) && (keyCode == 'x' || keyCode == 'X'))
	{
		PasteToSelection();
		m_shapeImpl->ClearSelectedShape();
	}
	else if (m_stage->GetKeyState(WXK_CONTROL) && (keyCode == 'c' || keyCode == 'C'))
	{
		clearClipboard();

		std::vector<IShape*> shapes;
		m_selection->Traverse(FetchAllVisitor<IShape>(shapes));
		for (size_t i = 0, n = shapes.size(); i < n; ++i)
			m_clipboard.push_back(shapes[i]->Clone());
	}
	else if (m_stage->GetKeyState(WXK_CONTROL) && (keyCode == 'v' || keyCode == 'V'))
	{
		for (size_t i = 0, n = m_clipboard.size(); i < n; ++i) {
			InsertShapeSJ::Instance()->Insert(m_clipboard[i]->Clone());
		}
	}

	return false;
}

bool SelectShapesOP::OnMouseLeftDown(int x, int y)
{
	m_bDraggable = true;

	m_move_last_pos.setInvalid();

	Vector pos = m_stage->TransPosScrToProj(x, y);
	IShape* selected = m_shapeImpl->QueryShapeByPos(pos);
	if (selected)
	{
		if (m_stage->GetKeyState(WXK_CONTROL))
		{
			if (m_selection->IsExist(selected)) {
				m_selection->Remove(selected);
			} else {
				m_selection->Add(selected);
			}
			SelectShapeSetSJ::Instance()->Selecte(m_selection, m_shapeImpl);
		}
		else
		{
			if (!m_selection->IsExist(selected))
			{
				m_selection->Clear();
				m_selection->Add(selected);
				SelectShapeSJ::Instance()->Select(selected, m_shapeImpl);
			} else {
				m_move_last_pos = pos;
			}
		}
		m_first_pos.setInvalid();

		if (m_callback)
			m_callback->updateControlValue();
	}
	else
	{
		DrawRectangleOP::OnMouseLeftDown(x, y);
		m_first_pos = pos;
		if (m_stage->GetKeyState(WXK_CONTROL))
			m_bDraggable = false;
		else
			m_selection->Clear();
	}

	return false;
}

bool SelectShapesOP::OnMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::OnMouseLeftUp(x, y)) return true;

	m_bDraggable = true;

	if (m_first_pos.isValid())
	{
		Rect rect(m_first_pos, m_stage->TransPosScrToProj(x, y));
		std::vector<IShape*> shapes;
		m_shapeImpl->QueryShapesByRect(rect, shapes);
		for (size_t i = 0, n = shapes.size(); i < n; ++i)
			m_selection->Add(shapes[i]);

		SelectShapeSetSJ::Instance()->Selecte(m_selection, m_shapeImpl);

		m_first_pos.setInvalid();

		if (m_callback) {
			m_callback->updateControlValue();
		}
	}

	m_stage->SetCanvasDirty();

	return false;
}

bool SelectShapesOP::OnMouseDrag(int x, int y)
{
	if (DrawRectangleOP::OnMouseDrag(x, y)) return true;

	if (!m_selection->IsEmpty() && m_move_last_pos.isValid()) {
		d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
		m_selection->Traverse(TranslateVisitor(pos - m_move_last_pos));
		m_move_last_pos = pos;
	}

	return !m_bDraggable;
}

bool SelectShapesOP::OnDraw() const
{
	if (DrawRectangleOP::OnDraw()) return true;

	m_selection->Traverse(DrawSelectedShapeVisitor());

	return false;
}

bool SelectShapesOP::Clear()
{
	if (DrawRectangleOP::Clear()) return true;

	clearClipboard();
	m_selection->Clear();
	m_first_pos.setInvalid();

	return false;
}

void SelectShapesOP::clearClipboard()
{
 	for (size_t i = 0, n = m_clipboard.size(); i < n; ++i)
 		m_clipboard[i]->Release();
 	m_clipboard.clear();
}

void SelectShapesOP::PasteToSelection() const
{
	std::vector<IShape*> shapes;
	m_selection->Traverse(FetchAllVisitor<IShape>(shapes));
	for (size_t i = 0, n = shapes.size(); i < n; ++i) {
		m_clipboard.push_back(shapes[i]->Clone());
	}
}

void SelectShapesOP::CopyFromSelection()
{
	for (size_t i = 0, n = m_clipboard.size(); i < n; ++i) {
		InsertShapeSJ::Instance()->Insert(m_clipboard[i]->Clone());
	}
}

//////////////////////////////////////////////////////////////////////////
// class TranslateSpriteState::TranslateVisitor
//////////////////////////////////////////////////////////////////////////

void SelectShapesOP::TranslateVisitor::
Visit(Object* object, bool& bFetchNext)
{
	d2d::IShape* shape = static_cast<d2d::IShape*>(object);
	shape->Translate(m_offset);
	bFetchNext = true;
}

}