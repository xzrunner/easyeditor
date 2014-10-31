#include "StageCanvas.h"
#include "StagePanel.h"
#include "BuildingCfg.h"
#include "SymbolExt.h"
#include "SymbolRender.h"
#include "tools.h"

static const d2d::Colorf LIGHT_GRAY = d2d::Colorf(0.8f, 0.8f, 0.8f);

namespace sg
{

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::OnTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::SpriteStageCanvas(stage, stage)
	, m_stage(stage)
	, m_background(NULL)
	, m_timer(this, TIMER_ID)
{
	m_timer.Start(1000 / 30);

	SymbolRender::Instance()->Init(stage);
}

StageCanvas::~StageCanvas()
{
	if (m_background) {
		m_background->release();
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
	ret.y *= 0.75f;
	return ret;
}

d2d::Vector StageCanvas::TransToFlatView(const d2d::Vector& pos)
{
	d2d::Vector ret = pos;
	ret.y /= 0.75f;
	ret = d2d::Math::rotateVector(ret, - d2d::PI / 4);
	return ret;
}

void StageCanvas::onDraw()
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
	}

	m_editPanel->drawEditTemp();

//	m_stage->getCheckBoard().DebugDraw();
}

void StageCanvas::OnTimer(wxTimerEvent& event)
{
	Refresh();
}

void StageCanvas::DrawBackground() const
{
	if (m_background) {
		d2d::SpriteDraw::drawSprite(m_background);
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
			d2d::PrimitiveDraw::drawLine(d2d::Vector(0, i*edge), d2d::Vector(width, i*edge), LIGHT_GRAY);
		}
		for (int i = 0; i <= col; ++i) {
			d2d::PrimitiveDraw::drawLine(d2d::Vector(i*edge, 0), d2d::Vector(i*edge, height), LIGHT_GRAY);
		}
	}
	else
	{
		for (int i = 0; i <= row; ++i) {
			d2d::Vector s = TransToBirdView(d2d::Vector(0, i*edge));
			d2d::Vector e = TransToBirdView(d2d::Vector(width, i*edge));
			d2d::PrimitiveDraw::drawLine(s, e, LIGHT_GRAY);
		}
		for (int i = 0; i <= col; ++i) {
			d2d::Vector s = TransToBirdView(d2d::Vector(i*edge, 0));
			d2d::Vector e = TransToBirdView(d2d::Vector(i*edge, height));
			d2d::PrimitiveDraw::drawLine(s, e, LIGHT_GRAY);
		}
	}
}

void StageCanvas::DrawGrass() const
{
	std::vector<d2d::ISprite*> sprites;
	m_stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		d2d::ISprite* s = sprites[i];
		SymbolRender::Instance()->DrawGrass(s->getSymbol(), s->getPosition(), m_stage->GetPerspective());
	}
}

void StageCanvas::DrawGrids() const
{
	std::vector<d2d::ISprite*> sprites;
	m_stage->getSpriteSelection()->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		d2d::ISprite* s = sprites[i];
		bool valid = m_stage->GetCheckBoard().IsValid(s);
		SymbolRender::Instance()->DrawGrids(s->getSymbol(), s->getPosition(), valid, m_stage->GetPerspective());
	}
}

void StageCanvas::DrawSprites() const
{
	std::vector<d2d::ISprite*> walls;

	std::vector<d2d::ISprite*> sprites;
	m_stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites), d2d::e_visible);
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		if (IsSymbolWall(*sprite)) {
			walls.push_back(sprite);
		} else {
			d2d::SpriteDraw::drawSprite(sprite);
		}
	}

	std::sort(walls.begin(), walls.end(), d2d::SpriteCmp(d2d::SpriteCmp::e_y_invert));
	for (int i = 0, n = walls.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = walls[i];
		SymbolExt* info = static_cast<SymbolExt*>(sprite->getSymbol().getUserData());
		{
			if (info->wall_type == 0) {
				d2d::Vector pos = sprite->getPosition() + d2d::Vector(0, 4);
				d2d::SpriteDraw::drawSprite(&sprite->getSymbol(), d2d::Matrix(), pos);
			} else if (info->wall_type == 1) {
				d2d::Vector pos = sprite->getPosition() + d2d::Vector(-10, 8);
				d2d::SpriteDraw::drawSprite(&sprite->getSymbol(), d2d::Matrix(), pos);
			} else if (info->wall_type == 2) {
				d2d::Vector pos = sprite->getPosition() + d2d::Vector(10, 8);
				d2d::SpriteDraw::drawSprite(&sprite->getSymbol(), d2d::Matrix(), pos);
			} else if (info->wall_type == 3) {
				d2d::Vector pos = sprite->getPosition() + d2d::Vector(0, 6);
				d2d::SpriteDraw::drawSprite(&sprite->getSymbol(), d2d::Matrix(), pos);
			}
		}
	}
}

void StageCanvas::DrawArrow() const
{
	std::vector<d2d::ISprite*> sprites;
	m_stage->getSpriteSelection()->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	if (sprites.size() == 1) {
		d2d::ISprite* s = sprites[0];
		SymbolRender::Instance()->DrawArrow(s->getSymbol(), s->getPosition());
	}
}

}
