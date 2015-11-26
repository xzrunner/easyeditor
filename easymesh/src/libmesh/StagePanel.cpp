#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "EditShape.h"

#include <easyshape.h>

namespace emesh
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiShapesImpl()
	, m_background(NULL)
{
	m_symbol = new Symbol;
	init(NULL);

	InitSubjects();
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiShapesImpl()
	, m_background(NULL)
{
	m_symbol = new Symbol;
	init(library);

	InitSubjects();
}

// StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
// 					   d2d::LibraryPanel* library, Sprite* sprite)
// 	: d2d::EditPanel(parent, frame)
// 	, d2d::MultiShapesImpl()
// 	, m_background(NULL)
// {
// 	sprite->Retain();
// 	m_sprite = sprite;
// 
// 	init(library);
// }

StagePanel::~StagePanel()
{
	m_symbol->Release();
	if (m_background) {
		m_background->Release();
	}

	for (int i = 0, n = m_subjects.size(); i < n; ++i) {
		m_subjects[i]->UnRegister(this);
	}
}

void StagePanel::Notify(int sj_id, void* ud)
{
	d2d::MultiShapesImpl::Notify(sj_id, ud);

	switch (sj_id) 
	{
	case d2d::MSG_REMOVE_SHAPE:
		if (Shape* shape = m_symbol->getShape()) {
			if (static_cast<EditShape*>(shape)->RemoveShape((d2d::IShape*)ud)) {
				d2d::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
		break;
	case d2d::MSG_INSERT_SHAPE:
		if (Shape* shape = m_symbol->getShape()) {
			if (static_cast<EditShape*>(shape)->InsertShape((d2d::IShape*)ud)) {
				d2d::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
		break;
	case d2d::MSG_CLEAR_SHAPE:
		if (Shape* shape = m_symbol->getShape()) {
			if (static_cast<EditShape*>(shape)->ClearShape()) {
				d2d::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
		break;
	case d2d::MSG_CLEAR_PANEL:
		m_symbol->Release();
		m_symbol = new Symbol;
		break;
	}
}

void StagePanel::TraverseShapes(d2d::IVisitor& visitor, 
								d2d::DataTraverseType type/* = d2d::DT_ALL*/) const
{
	Shape* shape = m_symbol->getShape();
	if (shape) {
		static_cast<EditShape*>(shape)->TraverseShape(visitor);
	}
}

void StagePanel::SetSymbol(Symbol* symbol)
{
	if (m_symbol != symbol) {
		m_symbol->Release();
		m_symbol = symbol;
		m_symbol->Retain();
	}
}

const Symbol* StagePanel::GetSymbol() const
{
	return m_symbol;
}

Shape* StagePanel::GetShape()
{
	return m_symbol->getShape();
}

void StagePanel::LoadFromSymbol(const d2d::ISymbol* symbol)
{
}

void StagePanel::UpdateSymbol()
{
	if (Shape* shape = m_symbol->getShape()) {
		std::vector<const libshape::ChainShape*> polylines;
		TraverseShapes(d2d::FetchAllVisitor<const libshape::ChainShape>(polylines));
		shape->Refresh();
	}
}

void StagePanel::CreateShape()
{
	if (m_symbol) {
		m_symbol->CreateShape();
	}
}

void StagePanel::init(d2d::LibraryPanel* library)
{
	SetEditOP(new d2d::ZoomViewOP(this, GetStageImpl(), true));
	SetCanvas(new StageCanvas(this));

	if (library) {
		SetDropTarget(new StageDropTarget(this, library));
	}
}

void StagePanel::InitSubjects()
{
	m_subjects.push_back(d2d::ClearPanelSJ::Instance());
	m_subjects.push_back(d2d::RemoveShapeSJ::Instance());
	m_subjects.push_back(d2d::InsertShapeSJ::Instance());
	m_subjects.push_back(d2d::ClearShapeSJ::Instance());
	for (int i = 0, n = m_subjects.size(); i < n; ++i) {
		m_subjects[i]->Register(this);
	}
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::StageDropTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::StageDropTarget::
StageDropTarget(StagePanel* stage, d2d::LibraryPanel* library)
	: d2d::StageDropTarget(stage, stage->GetStageImpl(), library)
	, m_stage(stage)
{
}

bool StagePanel::StageDropTarget::
OnDropSymbol(d2d::ISymbol* symbol, const d2d::Vector& pos)
{
	if (d2d::ImageSymbol* image = dynamic_cast<d2d::ImageSymbol*>(symbol))
	{
		Symbol* symbol = new Symbol(image->getImage());
		m_stage->m_symbol->Release();
		m_stage->m_symbol = symbol;
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
		d2d::ResetViewportSJ::Instance()->Reset();

		return true;
	}

	return false;
}

}