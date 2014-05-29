#include "StageCanvas.h"
#include "StagePanel.h"
#include "ResourceMgr.h"

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
	d2d::SpriteStageCanvas::onDraw();
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

}
