#include "StageCanvas.h"
#include "StagePanel.h"

#include "frame/SettingCfg.h"
#include "frame/config.h"

namespace lr
{

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(StagePanel* statge)
	: d2d::OrthoCanvas(statge)
	, m_statge(statge)
	, m_timer(this, TIMER_ID)
{
	m_timer.Start(100);
}

void StageCanvas::onDraw()
{
	m_statge->traverseSprites(d2d::DrawSpritesVisitor(), d2d::DT_VISIBLE);
	m_statge->traverseShapes(d2d::DrawShapesVisitor(), d2d::DT_VISIBLE);

	DrawGuideLines();

	m_editPanel->drawEditTemp();
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}

void StageCanvas::DrawGuideLines() const
{
	SettingCfg* cfg = SettingCfg::Instance();

	d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), 
		cfg->m_view_width * 0.5f,
		cfg->m_view_height * 0.5f,
		d2d::LIGHT_GREY_LINE);

	d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), 
		MAP_EDGE_LEN * 0.5f,
		MAP_EDGE_LEN * 0.5f,
		d2d::LIGHT_RED_LINE);

	m_statge->DebugDraw();
}

}