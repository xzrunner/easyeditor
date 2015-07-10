#include "SelectShapesOP.h"

#include "common/visitors.h"
#include "common/Rect.h"
#include "view/MultiShapesImpl.h"
#include "component/AbstractEditCMPT.h"
#include "render/DrawSelectedShapeVisitor.h"

namespace d2d
{

SelectShapesOP::SelectShapesOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl,
							   ViewPanelMgr* view_panel_mgr /*= NULL*/,
							   AbstractEditCMPT* callback/* = NULL*/)
	: DrawRectangleOP(editPanel)
	, m_callback(callback)
	, m_shapeImpl(shapesImpl)
	, m_bDraggable(true)
	, m_view_panel_mgr(view_panel_mgr)
{
	m_selection = shapesImpl->GetShapeSelection();
	m_selection->Retain();

	m_firstPos.setInvalid();
}

SelectShapesOP::~SelectShapesOP()
{
	ClearClipboard();

 	m_selection->Clear();
 	m_selection->Release();
}

bool SelectShapesOP::OnKeyDown(int keyCode)
{
	if (DrawRectangleOP::OnKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
	{
		m_shapeImpl->ClearShapeSelection();
		Clear();
	}
	else if (wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_CONTROL_X))
	{
		PasteToSelection();
		m_shapeImpl->ClearShapeSelection();
		return true;
	}
	else if (wxGetKeyState(WXK_CONTROL) && (keyCode == 'c' || keyCode == 'C'))
	{
		PasteToSelection();
		return true;
	}
	else if (wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_CONTROL_V))
	{
		CopyFromSelection();
		return true;
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
		if (wxGetKeyState(WXK_CONTROL))
		{
			if (m_selection->IsExist(selected)) {
				m_selection->Remove(selected);
			} else {
				m_selection->Add(selected);
			}
			if (m_view_panel_mgr) {
				m_view_panel_mgr->SelectMultiShapes(m_selection, m_shapeImpl);
			}
		}
		else
		{
			if (!m_selection->IsExist(selected))
			{
				m_selection->Clear();
				m_selection->Add(selected);
				if (m_view_panel_mgr) {
					m_view_panel_mgr->SelectShape(selected, m_shapeImpl);
				}
			} else {
				m_move_last_pos = pos;
			}
		}
		m_firstPos.setInvalid();

		if (m_callback)
			m_callback->updateControlValue();
	}
	else
	{
		DrawRectangleOP::OnMouseLeftDown(x, y);
		m_firstPos = pos;
		if (wxGetKeyState(WXK_CONTROL))
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

	if (m_firstPos.isValid())
	{
		Rect rect(m_firstPos, m_stage->TransPosScrToProj(x, y));
		std::vector<IShape*> shapes;
		m_shapeImpl->QueryShapesByRect(rect, shapes);
		for (size_t i = 0, n = shapes.size(); i < n; ++i)
			m_selection->Add(shapes[i]);

		if (m_view_panel_mgr) {
			m_view_panel_mgr->SelectMultiShapes(m_selection, m_shapeImpl);
		}

		m_firstPos.setInvalid();

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

	ClearClipboard();
	m_selection->Clear();
	m_firstPos.setInvalid();

	return false;
}

void SelectShapesOP::ClearClipboard()
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
		m_shapeImpl->InsertShape(m_clipboard[i]->Clone());
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