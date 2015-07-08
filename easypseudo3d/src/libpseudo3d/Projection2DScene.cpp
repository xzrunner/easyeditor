#include "Projection2DScene.h"
#include "StageCanvas2D.h"
#include "Proj2DEditOP.h"

namespace epseudo3d
{

Projection2DScene::Projection2DScene(d2d::EditPanel* stage)
	: m_stage(stage)
{
}

Projection2DScene::~Projection2DScene()
{
}

void Projection2DScene::Load(const char* filename)
{
	static const int DIS = 600;

	d2d::SymbolMgr* sym_mgr = d2d::SymbolMgr::Instance();
	d2d::SpriteFactory* spr_fac = d2d::SpriteFactory::Instance();

	m_buildings.push_back(sym_mgr->FetchSymbol("mmzb/1002_3_lv5.png"));
	m_positions.push_back(d2d::Vector(0, 0));

	m_buildings.push_back(sym_mgr->FetchSymbol("mmzb/1005_3_lv12.png"));
	m_positions.push_back(d2d::Vector(DIS, 0));

	m_buildings.push_back(sym_mgr->FetchSymbol("mmzb/b0013_base.png"));
	m_positions.push_back(d2d::Vector(DIS * 2, 0));

	m_buildings.push_back(sym_mgr->FetchSymbol("mmzb/b0016_base.png"));
	m_positions.push_back(d2d::Vector(0, DIS));

	m_buildings.push_back(sym_mgr->FetchSymbol("mmzb/b0017_base.png"));
	m_positions.push_back(d2d::Vector(DIS, DIS));

	m_buildings.push_back(sym_mgr->FetchSymbol("mmzb/b0021_base.png"));
	m_positions.push_back(d2d::Vector(DIS * 2, DIS));

	InitCamera();
	InitEditOP();
}

void Projection2DScene::Draw() const
{
	assert(m_buildings.size() == m_positions.size());
	for (int i = 0, n = m_buildings.size(); i < n; ++i) {
		d2d::ISymbol* sym = m_buildings[i];

		d2d::Vector pos;
		float scale;
		CalProjInfo(m_positions[i], &pos, &scale);

		d2d::SpriteRenderer::Instance()->Draw(sym, d2d::Matrix(), pos, 0, scale, scale);
	}
}

void Projection2DScene::InitCamera()
{
	d2d::Camera* cam = m_stage->GetCamera();
	cam->Translate(d2d::Vector(600, 0));
	cam->SetScale(2);
	m_stage->RefreshStage();
}

void Projection2DScene::InitEditOP()
{
	static_cast<Proj2DEditOP*>(m_stage->GetEditOP())->InitPlayer();
}

void Projection2DScene::CalProjInfo(const d2d::Vector& src_pos, d2d::Vector* dst_pos, float* dst_scale) const
{
	static const int VIEW_DIS = 600;

	d2d::Camera* cam = m_stage->GetCamera();
	const d2d::Vector& cam_pos = cam->GetPosition();

	float x = cam_pos.x;
	float y = cam_pos.y;
	float y_fix = cam_pos.y + VIEW_DIS;

	*dst_scale = std::min(1.0f, VIEW_DIS / fabs(y_fix - src_pos.y));

	dst_pos->x = VIEW_DIS * (src_pos.x - x) / fabs(y - src_pos.y) + x;
	dst_pos->y = src_pos.y;
}

}