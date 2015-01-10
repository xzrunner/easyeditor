#include "StageCanvas.h"
#include "StagePanel.h"
#include "Context.h"
#include "config.h"

using namespace epacker;

BEGIN_EVENT_TABLE(StageCanvas, d2d::PerspectCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage)
	, m_stage_impl(stage)
	, m_timer(this, TIMER_ID)
{
	m_timer.Start(100);
}

void StageCanvas::onDraw()
{
	m_stage_impl->traverseSprites(d2d::DrawSpritesVisitor(), d2d::e_visible);

	drawRegion();
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}

void StageCanvas::drawRegion()
{
	const float width = Context::Instance()->width,
		height = Context::Instance()->height;

	StagePanel* stage = static_cast<StagePanel*>(m_editPanel);

	// bg
	d2d::PrimitiveDraw::rect(
		d2d::Vector(0, 0), 
		d2d::Vector(width, height), 
		d2d::LIGHT_RED_LINE);

	int x = 0, y = 0;
	int count = stage->GetTextureAccount();
	for (int i = 0; i < count; ++i)
	{
		d2d::PrimitiveDraw::rect(d2d::Vector(x, y), 
			d2d::Vector(x + width, y + height), 
			d2d::LIGHT_GREY_LINE);
		x += Context::Instance()->width * TEXTURE_X_OFFSET_FACTOR;
	}

	// physics
	stage->drawPhysics();
}