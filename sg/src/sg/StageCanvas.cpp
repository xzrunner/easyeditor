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
	d2d::SpriteStageCanvas::onDraw();
	drawArrow();
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
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		SymbolInfo* info = static_cast<SymbolInfo*>(sprite->getSymbol().getUserData());
		if (info == NULL) {
			continue;
		}

		d2d::ISprite* grass = m_stage->m_grass[info->size - 1];
		grass->setTransform(sprite->getPosition(), 0);
		d2d::SpriteDraw::drawSprite(grass);
	}
}

void StageCanvas::drawGrids() const
{
	d2d::ISprite* grid = m_stage->m_grid;

	std::vector<d2d::ISprite*> sprites;
	m_stage->getSpriteSelection()->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		int row, col;
		m_stage->transCoordsToGridPos(sprite->getPosition(), row, col);

		SymbolInfo* info = static_cast<SymbolInfo*>(sprite->getSymbol().getUserData());
		int center = (info->size >> 1);
		for (int i = 0; i < info->size; ++i) {
			for (int j = 0; j < info->size; ++j) {
				d2d::Vector pos;
				m_stage->transGridPosToCoords(row + i - center, col + j - center, pos);
				grid->setTransform(pos, grid->getAngle());
//				grid->multiCol = d2d::Colorf(0, 255, 0, 10);
				d2d::SpriteDraw::drawSprite(grid, d2d::Colorf(0, 255, 0, 10));
			}
		}
	}
}

void StageCanvas::drawArrow() const
{
	std::vector<d2d::ISprite*> sprites;
	m_stage->getSpriteSelection()->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	if (sprites.size() != 1) {
		return;
	}

	d2d::ISprite* sprite = sprites[0];
	SymbolInfo* info = static_cast<SymbolInfo*>(sprite->getSymbol().getUserData());
	if (info == NULL) {
		return;
	}
	int r = (info->size >> 1) + 2;

	int row, col;
	m_stage->transCoordsToGridPos(sprite->getPosition(), row, col);
	// left
	{
		d2d::Vector pos;
		m_stage->transGridPosToCoords(row, col - r, pos);
		d2d::SpriteDraw::drawSprite(m_stage->m_arrow_right, pos, d2d::PI);
	}
	// right
	{
		d2d::Vector pos;
		m_stage->transGridPosToCoords(row, col + r, pos);
		d2d::SpriteDraw::drawSprite(m_stage->m_arrow_right, pos);
	}
	// up
	{
		d2d::Vector pos;
		m_stage->transGridPosToCoords(row + r, col, pos);
		d2d::SpriteDraw::drawSprite(m_stage->m_arrow_down, pos, d2d::PI);
	}
	// down
	{
		d2d::Vector pos;
		m_stage->transGridPosToCoords(row - r, col, pos);
		d2d::SpriteDraw::drawSprite(m_stage->m_arrow_down, pos);
	}
}

}
