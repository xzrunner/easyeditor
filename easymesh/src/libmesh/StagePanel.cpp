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
	m_sym = new Symbol;

	ee::EditOP* editop = new ee::ZoomViewOP(this, GetStageImpl(), true);
	SetEditOP(editop);
	editop->RemoveReference();

	ee::StageCanvas* canvas = new StageCanvas(this, glctx);
	SetCanvas(canvas);
	canvas->RemoveReference();

	if (library) {
		SetDropTarget(new StageDropTarget(this, library));
	}

	InitSubjects();
}

StagePanel::~StagePanel()
{
	m_sym->RemoveReference();
	if (m_background) {
		m_background->RemoveReference();
	}
}

void StagePanel::TraverseShapes(ee::Visitor<ee::Shape>& visitor, 
								ee::DataTraverseType type/* = ee::DT_ALL*/) const
{
	Mesh* mesh = m_sym->GetMesh();
	if (mesh) {
		static_cast<EditableMesh*>(mesh)->TraverseMesh(visitor);
	}
}

void StagePanel::SetMeshSymbol(Symbol* sym)
{	
	cu::RefCountObjAssign(m_sym, sym);
}

const Symbol* StagePanel::GetMeshSymbol() const
{
	return m_sym;
}

Mesh* StagePanel::GetMesh()
{
	return m_sym->GetMesh();
}

void StagePanel::LoadFromSymbol(const ee::Symbol* sym)
{
}

void StagePanel::UpdateSymbol()
{
	if (Mesh* mesh = m_sym->GetMesh()) {
		std::vector<ee::Shape*> polylines;
		TraverseShapes(ee::FetchAllVisitor<ee::Shape>(polylines));
		mesh->Refresh();
	}
}

void StagePanel::RecreateMesh()
{
	const ee::Symbol* base_sym = m_sym->GetMesh()->GetBaseSymbol();
	Symbol* mesh_sym = new Symbol(const_cast<ee::Symbol*>(base_sym));
	m_sym->RemoveReference();
	m_sym = mesh_sym;
	GetEditOP()->Clear();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
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
		if (Mesh* mesh = m_sym->GetMesh()) {
			if (static_cast<EditableMesh*>(mesh)->RemoveMesh((ee::Shape*)ud)) {
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
		break;
	case ee::MSG_INSERT_SHAPE:
		if (Mesh* mesh = m_sym->GetMesh()) {
			if (static_cast<EditableMesh*>(mesh)->InsertMesh((ee::Shape*)ud)) {
				UpdateSymbol();
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
		break;
	case ee::MSG_CLEAR_SHAPE:
		if (Mesh* mesh = m_sym->GetMesh()) {
			if (static_cast<EditableMesh*>(mesh)->ClearMesh()) {
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
		break;
	case ee::MSG_CLEAR_PANEL:
		m_sym->RemoveReference();
		m_sym = new Symbol;
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
OnDropSymbol(ee::Symbol* sym, const sm::vec2& pos)
{
	Symbol* mesh_sym = new Symbol(sym);
	m_stage->m_sym->RemoveReference();
	m_stage->m_sym = mesh_sym;
	m_stage->GetEditOP()->Clear();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
	return true;
}

}