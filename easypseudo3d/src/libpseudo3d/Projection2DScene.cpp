#include "Projection2DScene.h"
#include "StageCanvas2D.h"
#include "Proj2DEditOP.h"

#include <ee/SymbolMgr.h>
#include <ee/SpriteFactory.h>
#include <ee/SpriteRenderer.h>
#include <ee/EditPanelImpl.h>
#include <ee/Camera.h>
#include <ee/panel_msg.h>

#include <sprite2/NodeRenderer.h>

namespace epseudo3d
{

Projection2DScene::Projection2DScene(ee::EditPanelImpl* stage)
	: m_stage(stage)
{
}

Projection2DScene::~Projection2DScene()
{
}

void Projection2DScene::Load(const char* filename)
{
	static const int DIS = 600;

	ee::SymbolMgr* sym_mgr = ee::SymbolMgr::Instance();
	ee::SpriteFactory* spr_fac = ee::SpriteFactory::Instance();

	m_buildings.push_back(sym_mgr->FetchSymbol("mmzb/1002_3_lv5.png"));
	m_positions.push_back(sm::vec2(0, 0));

	m_buildings.push_back(sym_mgr->FetchSymbol("mmzb/1005_3_lv12.png"));
	m_positions.push_back(sm::vec2(DIS, 0));

	m_buildings.push_back(sym_mgr->FetchSymbol("mmzb/b0013_base.png"));
	m_positions.push_back(sm::vec2(DIS * 2, 0));

	m_buildings.push_back(sym_mgr->FetchSymbol("mmzb/b0016_base.png"));
	m_positions.push_back(sm::vec2(0, DIS));

	m_buildings.push_back(sym_mgr->FetchSymbol("mmzb/b0017_base.png"));
	m_positions.push_back(sm::vec2(DIS, DIS));

	m_buildings.push_back(sym_mgr->FetchSymbol("mmzb/b0021_base.png"));
	m_positions.push_back(sm::vec2(DIS * 2, DIS));

	InitCamera();
	InitEditOP();
}

void Projection2DScene::Draw() const
{
	assert(m_buildings.size() == m_positions.size());
	for (int i = 0, n = m_buildings.size(); i < n; ++i) {
		ee::Symbol* sym = m_buildings[i];

		sm::vec2 pos;
		float scale;
		CalProjInfo(m_positions[i], &pos, &scale);

		s2::NodeRenderer::Draw(sym, sm::mat4(), pos, 0, scale, scale);
	}
}

void Projection2DScene::InitCamera()
{
	ee::Camera* cam = m_stage->GetCamera();
	cam->Translate(sm::vec2(600, 0));
	cam->SetScale(2);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void Projection2DScene::InitEditOP()
{
	static_cast<Proj2DEditOP*>(m_stage->GetEditOP())->InitPlayer();
}

void Projection2DScene::CalProjInfo(const sm::vec2& src_pos, sm::vec2* dst_pos, float* dst_scale) const
{
	static const int VIEW_DIS = 600;

	ee::Camera* cam = m_stage->GetCamera();
	const sm::vec2& cam_pos = cam->GetPosition();

	float x = cam_pos.x;
	float y = cam_pos.y;
	float y_fix = cam_pos.y + VIEW_DIS;

	*dst_scale = std::min(1.0f, VIEW_DIS / fabs(y_fix - src_pos.y));

	dst_pos->x = VIEW_DIS * (src_pos.x - x) / fabs(y - src_pos.y) + x;
	dst_pos->y = src_pos.y;
}

}