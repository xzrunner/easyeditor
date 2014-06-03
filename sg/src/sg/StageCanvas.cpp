#include "StageCanvas.h"
#include "StagePanel.h"
#include "ResourceMgr.h"
#include "SymbolInfo.h"

static const d2d::Colorf LIGHT_GRAY = d2d::Colorf(0.8f, 0.8f, 0.8f);

namespace sg
{

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(StagePanel* parent)
	: d2d::SpriteStageCanvas(parent, parent)
	, m_stage(parent)
	, m_background(NULL)
	, m_timer(this, TIMER_ID)
	, m_render(parent)
{
	m_timer.Start(1000 / 30);
}

StageCanvas::~StageCanvas()
{
	if (m_background) {
		m_background->release();
	}
}

void StageCanvas::setBackground(d2d::ISymbol* background)
{
	m_background = d2d::SpriteFactory::Instance()->create(background);
	m_background->translate(d2d::Vector(0.0f, m_background->getBounding()->height() * 0.375f));
}

d2d::Vector StageCanvas::transToBirdView(const d2d::Vector& pos)
{
	d2d::Vector ret = d2d::Math::rotateVector(pos, d2d::PI / 4);
	ret.y *= 0.75f;
	return ret;
}

d2d::Vector StageCanvas::transToFlatView(const d2d::Vector& pos)
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
		m_stage->getResourceMgr()->init();
		inited = true;
	}

	drawBackground();
	drawGuideLines();
	drawGrass();
	drawGrids();
	drawSprites();
	drawArrow();

	m_editPanel->drawEditTemp();

//	m_stage->getCheckBoard().DebugDraw();
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}

void StageCanvas::drawBackground() const
{
	if (m_background) {
		d2d::SpriteDraw::drawSprite(m_background);
	}
}

void StageCanvas::drawGuideLines() const
{
	int row, col, edge;
	StagePanel* stage = static_cast<StagePanel*>(m_editPanel);
	stage->getLayoutInfo(row, col, edge);

	bool is_flat = stage->getPerspective();

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
			d2d::Vector s = transToBirdView(d2d::Vector(0, i*edge));
			d2d::Vector e = transToBirdView(d2d::Vector(width, i*edge));
			d2d::PrimitiveDraw::drawLine(s, e, LIGHT_GRAY);
		}
		for (int i = 0; i <= col; ++i) {
			d2d::Vector s = transToBirdView(d2d::Vector(i*edge, 0));
			d2d::Vector e = transToBirdView(d2d::Vector(i*edge, height));
			d2d::PrimitiveDraw::drawLine(s, e, LIGHT_GRAY);
		}
	}
}

void StageCanvas::drawGrass() const
{
	std::vector<d2d::ISprite*> sprites;
	m_stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		d2d::ISprite* s = sprites[i];
		m_render.DrawGrass(s->getSymbol(), s->getPosition());
	}
}

void StageCanvas::drawGrids() const
{
	std::vector<d2d::ISprite*> sprites;
	m_stage->getSpriteSelection()->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		d2d::ISprite* s = sprites[i];
		bool valid = m_stage->getCheckBoard().IsValid(s);
		m_render.DrawGrids(s->getSymbol(), s->getPosition(), valid);
	}
}

void StageCanvas::drawSprites() const
{
	std::vector<d2d::ISprite*> walls;

	std::vector<d2d::ISprite*> sprites;
	m_stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites), d2d::e_visible);
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		SymbolInfo* info = static_cast<SymbolInfo*>(sprite->getSymbol().getUserData());
		if (info->wall_type != -1) {
			walls.push_back(sprite);
		} else {
			d2d::SpriteDraw::drawSprite(sprite);
		}
	}

	std::sort(walls.begin(), walls.end(), d2d::SpriteCmp(d2d::SpriteCmp::e_y_invert));
	for (int i = 0, n = walls.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = walls[i];
		SymbolInfo* info = static_cast<SymbolInfo*>(sprite->getSymbol().getUserData());
		{
			if (info->wall_type == 0) {
				d2d::Vector pos = sprite->getPosition() + d2d::Vector(0, 4);
				d2d::SpriteDraw::drawSprite(&sprite->getSymbol(), pos);
			} else if (info->wall_type == 1) {
				d2d::Vector pos = sprite->getPosition() + d2d::Vector(-10, 8);
				d2d::SpriteDraw::drawSprite(&sprite->getSymbol(), pos);
			} else if (info->wall_type == 2) {
				d2d::Vector pos = sprite->getPosition() + d2d::Vector(10, 8);
				d2d::SpriteDraw::drawSprite(&sprite->getSymbol(), pos);
			} else if (info->wall_type == 3) {
				d2d::Vector pos = sprite->getPosition() + d2d::Vector(0, 6);
				d2d::SpriteDraw::drawSprite(&sprite->getSymbol(), pos);
			}
		}
	}
}

void StageCanvas::drawArrow() const
{
	std::vector<d2d::ISprite*> sprites;
	m_stage->getSpriteSelection()->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	if (sprites.size() == 1) {
		d2d::ISprite* s = sprites[0];
		m_render.DrawArrow(s->getSymbol(), s->getPosition());
	}
}

}
