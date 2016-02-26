#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "EditShape.h"

#include <easyshape.h>

#include <ee/FetchAllVisitor.h>
#include <ee/panel_msg.h>
#include <ee/shape_msg.h>
#include <ee/subject_id.h>
#include <ee/ImageSymbol.h>

namespace emesh
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: ee::EditPanel(parent, frame)
	, ee::MultiShapesImpl()
	, m_background(NULL)
{
	m_symbol = new Symbol;
	Init(NULL);

	InitSubjects();
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::MultiShapesImpl()
	, m_background(NULL)
{
	m_symbol = new Symbol;
	Init(library);

	InitSubjects();
}

// StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
// 					   ee::LibraryPanel* library, Sprite* sprite)
// 	: ee::EditPanel(parent, frame)
// 	, ee::MultiShapesImpl()
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
}

void StagePanel::TraverseShapes(ee::Visitor& visitor, 
								ee::DataTraverseType type/* = ee::DT_ALL*/) const
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

void StagePanel::LoadFromSymbol(const ee::Symbol* symbol)
{
}

void StagePanel::UpdateSymbol()
{
	if (Shape* shape = m_symbol->getShape()) {
		std::vector<const eshape::ChainShape*> polylines;
		TraverseShapes(ee::FetchAllVisitor<const eshape::ChainShape>(polylines));
		shape->Refresh();
	}
}

void StagePanel::CreateShape()
{
	if (m_symbol) {
		m_symbol->CreateShape();
	}
}

void StagePanel::Init(ee::LibraryPanel* library)
{
	SetEditOP(new ee::ZoomViewOP(this, GetStageImpl(), true));
	SetCanvas(new StageCanvas(this));

	if (library) {
		SetDropTarget(new StageDropTarget(this, library));
	}
}

void StagePanel::InitSubjects()
{
	RegistSubject(ee::ClearPanelSJ::Instance());
	RegistSubject(ee::RemoveShapeSJ::Instance());
	RegistSubject(ee::InsertShapeSJ::Instance());
	RegistSubject(ee::ClearShapeSJ::Instance());
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	ee::MultiShapesImpl::OnNotify(sj_id, ud);

	switch (sj_id) 
	{
	case ee::MSG_REMOVE_SHAPE:
		if (Shape* shape = m_symbol->getShape()) {
			if (static_cast<EditShape*>(shape)->RemoveShape((ee::Shape*)ud)) {
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
		break;
	case ee::MSG_INSERT_SHAPE:
		if (Shape* shape = m_symbol->getShape()) {
			if (static_cast<EditShape*>(shape)->InsertShape((ee::Shape*)ud)) {
				UpdateSymbol();
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
		break;
	case ee::MSG_CLEAR_SHAPE:
		if (Shape* shape = m_symbol->getShape()) {
			if (static_cast<EditShape*>(shape)->ClearShape()) {
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
		break;
	case ee::MSG_CLEAR_PANEL:
		m_symbol->Release();
		m_symbol = new Symbol;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::StageDropTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::StageDropTarget::
StageDropTarget(StagePanel* stage, ee::LibraryPanel* library)
	: ee::StageDropTarget(stage, stage->GetStageImpl(), library)
	, m_stage(stage)
{
}

bool StagePanel::StageDropTarget::
OnDropSymbol(ee::Symbol* symbol, const ee::Vector& pos)
{
	if (ee::ImageSymbol* image = dynamic_cast<ee::ImageSymbol*>(symbol))
	{
		Symbol* symbol = new Symbol(image->GetImage());
		m_stage->m_symbol->Release();
		m_stage->m_symbol = symbol;
		ee::SetCanvasDirtySJ::Instance()->SetDirty();

		return true;
	}

	return false;
}

}