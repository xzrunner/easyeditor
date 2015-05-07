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

	DrawRegion();
	DrawGuideLines();

	m_editPanel->drawEditTemp();
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}

void StageCanvas::DrawRegion() const
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

void StageCanvas::DrawGuideLines() const
{
	int row = 44, col = 44, edge = 48;

	bool is_flat = false;

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

}