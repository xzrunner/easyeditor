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

void StagePanel::SetMeshSymbol(Symbol* symbol)
{
	if (m_symbol != symbol) {
		m_symbol->Release();
		m_symbol = symbol;
		m_symbol->Retain();
	}
}

const Symbol* StagePanel::GetMeshSymbol() const
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

void StagePanel::RecreateMesh()
{
	const ee::Symbol* base_sym = m_symbol->GetMesh()->GetBaseSymbol();
	Symbol* mesh_sym = new Symbol(const_cast<ee::Symbol*>(base_sym));
	m_symbol->Release();
	m_symbol = mesh_sym;
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
OnDropSymbol(ee::Symbol* symbol, const sm::vec2& pos)
{
	Symbol* mesh_sym = new Symbol(symbol);
	m_stage->m_symbol->Release();
	m_stage->m_symbol = mesh_sym;
	m_stage->GetEditOP()->Clear();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
	return true;
}

}