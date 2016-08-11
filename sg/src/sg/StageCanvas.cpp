#include "StageCanvas.h"
#include "StagePanel.h"
#include "BuildingCfg.h"
#include "SymbolExt.h"
#include "SymbolRender.h"
#include "tools.h"

#include <ee/SpriteFactory.h>
#include <ee/BoundingBox.h>
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
	: ee::CameraCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
	, m_background(NULL)
{
	SymbolRender::Instance()->Init(stage);
}

StageCanvas::~StageCanvas()
{
	if (m_background) {
		m_background->Release();
	}
}

void StageCanvas::SetBackground(ee::Symbol* background)
{
	m_background = ee::SpriteFactory::Instance()->Create(background);
	m_background->Translate(sm::vec2(0.0f, m_background->GetBounding()->Height() * 0.375f));
}

sm::vec2 StageCanvas::TransToBirdView(const sm::vec2& pos)
{
	sm::vec2 ret = ee::Math2D::RotateVector(pos, SM_PI / 4);
	ret.y *= VIEW_TRANS;
	return ret;
}

sm::vec2 StageCanvas::TransToFlatView(const sm::vec2& pos)
{
	sm::vec2 ret = pos;
	ret.y /= VIEW_TRANS;
	ret = ee::Math2D::RotateVector(ret, - SM_PI / 4);
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
		ee::SpriteRenderer::Draw(m_background);
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
	std::vector<ee::Sprite*> sprites;
	m_stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ee::Sprite* s = sprites[i];
		SymbolRender::Instance()->DrawGrass(s->GetSymbol(), s->GetPosition(), m_stage->GetPerspective());
	}
}

void StageCanvas::DrawGrids() const
{
	std::vector<ee::Sprite*> sprites;
	m_stage->GetSpriteSelection()->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ee::Sprite* s = sprites[i];
		bool valid = m_stage->GetCheckBoard().IsValid(s);
		SymbolRender::Instance()->DrawGrids(s->GetSymbol(), s->GetPosition(), valid, m_stage->GetPerspective());
	}
}

void StageCanvas::DrawSprites() const
{
	std::vector<ee::Sprite*> sprites;
	m_stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites), ee::DT_VISIBLE);
	std::sort(sprites.begin(), sprites.end(), ee::SpriteCmp(ee::SpriteCmp::e_y_invert));
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::Sprite* sprite = sprites[i];
		if (IsSymbolWall(*sprite)) {
			SymbolExt* info = static_cast<SymbolExt*>(sprite->GetSymbol().GetUserData());
			{
				if (info->wall_type == 0) {
					sm::vec2 pos = sprite->GetPosition() + sm::vec2(0, 4);
					ee::SpriteRenderer::Draw(&sprite->GetSymbol(), sm::mat4(), pos);
				} else if (info->wall_type == 1) {
					sm::vec2 pos = sprite->GetPosition() + sm::vec2(-10, 8);
					ee::SpriteRenderer::Draw(&sprite->GetSymbol(), sm::mat4(), pos);
				} else if (info->wall_type == 2) {
					sm::vec2 pos = sprite->GetPosition() + sm::vec2(10, 8);
					ee::SpriteRenderer::Draw(&sprite->GetSymbol(), sm::mat4(), pos);
				} else if (info->wall_type == 3) {
					sm::vec2 pos = sprite->GetPosition() + sm::vec2(0, 6);
					ee::SpriteRenderer::Draw(&sprite->GetSymbol(), sm::mat4(), pos);
				}
			}
		} else {
			ee::SpriteRenderer::Draw(sprite);
		}
	}
}

void StageCanvas::DrawArrow() const
{
	std::vector<ee::Sprite*> sprites;
	m_stage->GetSpriteSelection()->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
	if (sprites.size() == 1) {
		ee::Sprite* s = sprites[0];
		SymbolRender::Instance()->DrawArrow(s->GetSymbol(), s->GetPosition());
	}
}

void StageCanvas::DrawAttackRegion() const
{
	std::vector<ee::Sprite*> sprites;
	m_stage->GetSpriteSelection()->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ee::Sprite* s = sprites[i];
		SymbolRender::Instance()->DrawRegion(s->GetSymbol(), s->GetPosition());
	}
}

}
