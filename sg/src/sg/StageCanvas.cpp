#include "StageCanvas.h"
#include "StagePanel.h"
#include "BuildingCfg.h"
#include "SymbolExt.h"
#include "SymbolRender.h"
#include "tools.h"

namespace sg
{

static const float VIEW_TRANS = 0.75f;

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage)
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

void StageCanvas::SetBackground(d2d::ISymbol* background)
{
	m_background = d2d::SpriteFactory::Instance()->create(background);
	m_background->translate(d2d::Vector(0.0f, m_background->getBounding()->height() * 0.375f));
}

d2d::Vector StageCanvas::TransToBirdView(const d2d::Vector& pos)
{
	d2d::Vector ret = d2d::Math::rotateVector(pos, d2d::PI / 4);
	ret.y *= VIEW_TRANS;
	return ret;
}

d2d::Vector StageCanvas::TransToFlatView(const d2d::Vector& pos)
{
	d2d::Vector ret = pos;
	ret.y /= VIEW_TRANS;
	ret = d2d::Math::rotateVector(ret, - d2d::PI / 4);
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
		d2d::SpriteRenderer::Instance()->Draw(m_background);
	}
}

void StageCanvas::DrawGuideLines() const
{
	int row, col, edge;
	m_stage->GetLayoutInfo(row, col, edge);

	bool is_flat = m_stage->GetPerspective();

	int width = col * edge;
	int height = row * edge;

	if (is_flat)
	{
		for (int i = 0; i <= row; ++i) {
			d2d::PrimitiveDraw::drawLine(d2d::Vector(0, i*edge), d2d::Vector(width, i*edge), d2d::LIGHT_GREY);
		}
		for (int i = 0; i <= col; ++i) {
			d2d::PrimitiveDraw::drawLine(d2d::Vector(i*edge, 0), d2d::Vector(i*edge, height), d2d::LIGHT_GREY);
		}
	}
	else
	{
		for (int i = 0; i <= row; ++i) {
			d2d::Vector s = TransToBirdView(d2d::Vector(0, i*edge));
			d2d::Vector e = TransToBirdView(d2d::Vector(width, i*edge));
			d2d::PrimitiveDraw::drawLine(s, e, d2d::LIGHT_GREY);
		}
		for (int i = 0; i <= col; ++i) {
			d2d::Vector s = TransToBirdView(d2d::Vector(i*edge, 0));
			d2d::Vector e = TransToBirdView(d2d::Vector(i*edge, height));
			d2d::PrimitiveDraw::drawLine(s, e, d2d::LIGHT_GREY);
		}
	}
}

void StageCanvas::DrawGrass() const
{
	std::vector<d2d::ISprite*> sprites;
	m_stage->TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		d2d::ISprite* s = sprites[i];
		SymbolRender::Instance()->DrawGrass(s->getSymbol(), s->getPosition(), m_stage->GetPerspective());
	}
}

void StageCanvas::DrawGrids() const
{
	std::vector<d2d::ISprite*> sprites;
	m_stage->GetSpriteSelection()->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		d2d::ISprite* s = sprites[i];
		bool valid = m_stage->GetCheckBoard().IsValid(s);
		SymbolRender::Instance()->DrawGrids(s->getSymbol(), s->getPosition(), valid, m_stage->GetPerspective());
	}
}

void StageCanvas::DrawSprites() const
{
	d2d::SpriteRenderer* rd = d2d::SpriteRenderer::Instance();

	std::vector<d2d::ISprite*> sprites;
	m_stage->TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites), d2d::DT_VISIBLE);
	std::sort(sprites.begin(), sprites.end(), d2d::SpriteCmp(d2d::SpriteCmp::e_y_invert));
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		if (IsSymbolWall(*sprite)) {
			SymbolExt* info = static_cast<SymbolExt*>(sprite->getSymbol().GetUserData());
			{
				if (info->wall_type == 0) {
					d2d::Vector pos = sprite->getPosition() + d2d::Vector(0, 4);
					rd->Draw(&sprite->getSymbol(), d2d::Matrix(), pos);
				} else if (info->wall_type == 1) {
					d2d::Vector pos = sprite->getPosition() + d2d::Vector(-10, 8);
					rd->Draw(&sprite->getSymbol(), d2d::Matrix(), pos);
				} else if (info->wall_type == 2) {
					d2d::Vector pos = sprite->getPosition() + d2d::Vector(10, 8);
					rd->Draw(&sprite->getSymbol(), d2d::Matrix(), pos);
				} else if (info->wall_type == 3) {
					d2d::Vector pos = sprite->getPosition() + d2d::Vector(0, 6);
					rd->Draw(&sprite->getSymbol(), d2d::Matrix(), pos);
				}
			}
		} else {
			rd->Draw(sprite);
		}
	}
}

void StageCanvas::DrawArrow() const
{
	std::vector<d2d::ISprite*> sprites;
	m_stage->GetSpriteSelection()->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	if (sprites.size() == 1) {
		d2d::ISprite* s = sprites[0];
		SymbolRender::Instance()->DrawArrow(s->getSymbol(), s->getPosition());
	}
}

void StageCanvas::DrawAttackRegion() const
{
	std::vector<d2d::ISprite*> sprites;
	m_stage->GetSpriteSelection()->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		d2d::ISprite* s = sprites[i];
		SymbolRender::Instance()->DrawRegion(s->getSymbol(), s->getPosition());
	}
}

}
