#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "EditableMesh.h"

#include <easyshape.h>

#include <ee/FetchAllVisitor.h>
#include <ee/panel_msg.h>
#include <ee/shape_msg.h>
#include <ee/subject_id.h>
#include <ee/ImageSymbol.h>

namespace emesh
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, wxGLContext* glctx,
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::MultiShapesImpl()
	, m_background(NULL)
{
	m_symbol = new Symbol;

	ee::EditOP* editop = new ee::ZoomViewOP(this, GetStageImpl(), true);
	SetEditOP(editop);
	editop->Release();

	ee::StageCanvas* canvas = new StageCanvas(this, glctx);
	SetCanvas(canvas);
	canvas->Release();

	if (library) {
		SetDropTarget(new StageDropTarget(this, library));
	}

	InitSubjects();
}

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
	Mesh* mesh = m_symbol->GetMesh();
	if (mesh) {
		static_cast<EditableMesh*>(mesh)->TraverseMesh(visitor);
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

Mesh* StagePanel::GetMesh()
{
	return m_symbol->GetMesh();
}

void StagePanel::LoadFromSymbol(const ee::Symbol* symbol)
{
}

void StagePanel::UpdateSymbol()
{
	if (Mesh* mesh = m_symbol->GetMesh()) {
		std::vector<const eshape::ChainShape*> polylines;
		TraverseShapes(ee::FetchAllVisitor<const eshape::ChainShape>(polylines));
		mesh->Refresh();
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
		if (Mesh* mesh = m_symbol->GetMesh()) {
			if (static_cast<EditableMesh*>(mesh)->RemoveMesh((ee::Shape*)ud)) {
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
		break;
	case ee::MSG_INSERT_SHAPE:
		if (Mesh* mesh = m_symbol->GetMesh()) {
			if (static_cast<EditableMesh*>(mesh)->InsertMesh((ee::Shape*)ud)) {
				UpdateSymbol();
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
		break;
	case ee::MSG_CLEAR_SHAPE:
		if (Mesh* mesh = m_symbol->GetMesh()) {
			if (static_cast<EditableMesh*>(mesh)->ClearMesh()) {
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