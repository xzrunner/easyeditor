#include "StageCanvas.h"
#include "StagePanel.h"
#include "BuildingCfg.h"
#include "SymbolExt.h"
#include "SymbolRender.h"
#include "tools.h"

#include <ee/SpriteFactory.h>
#include <ee/Math2D.h>
#include <ee/SpriteRenderer.h>
#include <ee/color_config.h>
#include <ee/FetchAllVisitor.h>
#include <ee/SpriteSelection.h>

#include <sprite2/S2_RVG.h>

#include <algorithm>

namespace sg
{

static const float VIEW_TRANS = 0.75f;

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), s2::CAM_ORTHO2D)
	, m_stage(stage)
	, m_background(NULL)
{
	SymbolRender::Instance()->Init(stage);
}

StageCanvas::~StageCanvas()
{
	if (m_background) {
		m_background->RemoveReference();
	}
}

void StageCanvas::SetBackground(ee::Symbol* background)
{
	m_background = ee::SpriteFactory::Instance()->Create(background);
	m_background->Translate(sm::vec2(0.0f, m_background->GetBounding()->Height() * 0.375f));
}

sm::vec2 StageCanvas::TransToBirdView(const sm::vec2& pos)
{
	sm::vec2 ret = sm::rotate_vector(pos, SM_PI / 4);
	ret.y *= VIEW_TRANS;
	return ret;
}

sm::vec2 StageCanvas::TransToFlatView(const sm::vec2& pos)
{
	sm::vec2 ret = pos;
	ret.y /= VIEW_TRANS;
	ret = sm::rotate_vector(ret, - SM_PI / 4);
	return ret;
}

void StageCanvas::OnDrawSprites() const
{
	static bool inited = false;
	if (!inited) {
		m_stage->GetBuildingCfg().InitAllData();
		inited = true;
	}

	bool is_flat = m_stage->GetPerspective();
	if (is_flat)
	{
		DrawGuideLines();
		DrawGrass();
		DrawGrids();
		DrawSprites();
	}
	else
	{
		DrawBackground();
		DrawGuideLines();
		DrawGrass();
		DrawGrids();
		DrawSprites();
		DrawArrow();
		DrawAttackRegion();
	}

	m_stage->DrawEditOP();

//	m_stage->getCheckBoard().DebugDraw();
}

void StageCanvas::DrawBackground() const
{
	if (m_background) {
		ee::SpriteRenderer::Instance()->Draw(m_background);
	}
}

void StageCanvas::DrawGuideLines() const
{
	int row, col, edge;
	m_stage->GetLayoutInfo(row, col, edge);

	bool is_flat = m_stage->GetPerspective();

	int width = col * edge;
	int height = row * edge;

	s2::RVG::SetColor(ee::LIGHT_GREY);
	if (is_flat)
	{
		for (int i = 0; i <= row; ++i) {
			s2::RVG::Line(sm::vec2(0, i*edge), sm::vec2(width, i*edge));
		}
		for (int i = 0; i <= col; ++i) {
			s2::RVG::Line(sm::vec2(i*edge, 0), sm::vec2(i*edge, height));
		}
	}
	else
	{
		for (int i = 0; i <= row; ++i) {
			sm::vec2 s = TransToBirdView(sm::vec2(0, i*edge));
			sm::vec2 e = TransToBirdView(sm::vec2(width, i*edge));
			s2::RVG::Line(s, e);
		}
		for (int i = 0; i <= col; ++i) {
			sm::vec2 s = TransToBirdView(sm::vec2(i*edge, 0));
			sm::vec2 e = TransToBirdView(sm::vec2(i*edge, height));
			s2::RVG::Line(s, e);
		}
	}
}

void StageCanvas::DrawGrass() const
{
	std::vector<ee::Sprite*> sprs;
	m_stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		ee::Sprite* s = sprs[i];
		SymbolRender::Instance()->DrawGrass(s->GetSymbol(), s->GetPosition(), m_stage->GetPerspective());
	}
}

void StageCanvas::DrawGrids() const
{
	std::vector<ee::Sprite*> sprs;
	m_stage->GetSpriteSelection()->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprs));
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		ee::Sprite* s = sprs[i];
		bool valid = m_stage->GetCheckBoard().IsValid(s);
		SymbolRender::Instance()->DrawGrids(s->GetSymbol(), s->GetPosition(), valid, m_stage->GetPerspective());
	}
}

void StageCanvas::DrawSprites() const
{
	std::vector<ee::Sprite*> sprs;
	m_stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs), ee::DT_VISIBLE);
	std::sort(sprs.begin(), sprs.end(), ee::SpriteCmp(ee::SpriteCmp::e_y_invert));
	for (int i = 0, n = sprs.size(); i < n; ++i)
	{
		ee::Sprite* spr = sprs[i];
		if (IsSymbolWall(*spr)) {
			SymbolExt* info = static_cast<SymbolExt*>(spr->GetSymbol()->GetUserData());
			{
				if (info->wall_type == 0) {
					sm::vec2 pos = spr->GetPosition() + sm::vec2(0, 4);
					ee::SpriteRenderer::Instance()->Draw(spr->GetSymbol(), S2_MAT(), pos);
				} else if (info->wall_type == 1) {
					sm::vec2 pos = spr->GetPosition() + sm::vec2(-10, 8);
					ee::SpriteRenderer::Instance()->Draw(spr->GetSymbol(), S2_MAT(), pos);
				} else if (info->wall_type == 2) {
					sm::vec2 pos = spr->GetPosition() + sm::vec2(10, 8);
					ee::SpriteRenderer::Instance()->Draw(spr->GetSymbol(), S2_MAT(), pos);
				} else if (info->wall_type == 3) {
					sm::vec2 pos = spr->GetPosition() + sm::vec2(0, 6);
					ee::SpriteRenderer::Instance()->Draw(spr->GetSymbol(), S2_MAT(), pos);
				}
			}
		} else {
			ee::SpriteRenderer::Instance()->Draw(spr);
		}
	}
}

void StageCanvas::DrawArrow() const
{
	std::vector<ee::Sprite*> sprs;
	m_stage->GetSpriteSelection()->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprs));
	if (sprs.size() == 1) {
		ee::Sprite* s = sprs[0];
		SymbolRender::Instance()->DrawArrow(s->GetSymbol(), s->GetPosition());
	}
}

void StageCanvas::DrawAttackRegion() const
{
	std::vector<ee::Sprite*> sprs;
	m_stage->GetSpriteSelection()->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprs));
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		ee::Sprite* s = sprs[i];
		SymbolRender::Instance()->DrawRegion(s->GetSymbol(), s->GetPosition());
	}
}

}
