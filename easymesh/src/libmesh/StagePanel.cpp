#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "Mesh.h"

#include <easyshape.h>

#include <ee/FetchAllVisitor.h>
#include <ee/panel_msg.h>
#include <ee/shape_msg.h>
#include <ee/subject_id.h>
#include <ee/ImageSymbol.h>

#include <sprite2/UpdateParams.h>
#include <sprite2/Mesh.h>

namespace emesh
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, wxGLContext* glctx,
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::MultiShapesImpl()
	, m_background(NULL)
{
	m_sym = std::make_shared<Symbol>();

	SetEditOP(std::make_shared<ee::ZoomViewOP>(this, GetStageImpl(), true));
	SetCanvas(std::make_shared<StageCanvas>(this, glctx));

	if (library) {
		SetDropTarget(new StageDropTarget(this, library));
	}

	InitSubjects();
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   wxGLContext* glctx, const ee::SprPtr& edited, 
					   const ee::MultiSpritesImpl* bg_sprites)
	: ee::EditPanel(parent, frame)
	, ee::MultiShapesImpl()
	, m_background(NULL)
{
	m_sym = std::make_shared<Symbol>();

	SetEditOP(std::make_shared<ee::ZoomViewOP>(this, GetStageImpl(), true));
	SetCanvas(std::make_shared<StageCanvas>(this, glctx, edited, bg_sprites));

	InitSubjects();
}

bool StagePanel::UpdateStage()
{
	return m_sym->Update(s2::UpdateParams(), 0);
}

void StagePanel::TraverseShapes(ee::RefVisitor<ee::Shape>& visitor,
								ee::DataTraverseType type/* = ee::DT_ALL*/) const
{
	auto mesh = static_cast<Mesh*>(m_sym->GetMesh().get());
	if (mesh) {
		mesh->TraverseMesh(visitor);
	}
}

void StagePanel::SetMeshSymbol(const std::shared_ptr<Symbol>& sym)
{	
	m_sym = sym;
}

const Symbol& StagePanel::GetMeshSymbol() const
{
	return *m_sym;
}

Mesh* StagePanel::GetMesh()
{
	return static_cast<Mesh*>(m_sym->GetMesh().get());
}

void StagePanel::LoadFromSymbol(const ee::SymConstPtr& sym)
{
}

void StagePanel::RecreateMesh()
{
	auto base_sym = std::const_pointer_cast<s2::Symbol>(m_sym->GetMesh()->GetBaseSymbol());
	auto mesh_sym = std::make_shared<Symbol>(std::dynamic_pointer_cast<ee::Symbol>(base_sym));
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
		if (Mesh* mesh = static_cast<Mesh*>(m_sym->GetMesh().get())) {
			if (mesh->RemoveMesh(*(ee::ShapePtr*)ud)) {
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
		break;
	case ee::MSG_INSERT_SHAPE:
		if (Mesh* mesh = static_cast<Mesh*>(m_sym->GetMesh().get())) {
			if (mesh->InsertMesh(*(ee::ShapePtr*)ud)) {
//				UpdateSymbol();
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
			}
		}
		break;
	case ee::MSG_CLEAR_SHAPE:
// 		if (Mesh* mesh = dynamic_cast<Mesh*>(m_sym->GetMesh())) {
// 			if (mesh->ClearMesh()) {
// 				ee::SetCanvasDirtySJ::Instance()->SetDirty();
// 			}
// 		}
		break;
	case ee::MSG_CLEAR_PANEL:
		m_sym = std::make_shared<Symbol>();
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
OnDropSymbol(const ee::SymPtr& sym, const sm::vec2& pos)
{
	auto mesh_sym = std::make_shared<Symbol>(sym);
	m_stage->m_sym = mesh_sym;
	m_stage->GetEditOP()->Clear();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
	return true;
}

}