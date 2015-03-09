#include "StageCanvas.h"
#include "StagePanel.h"

namespace etexture
{

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(StagePanel* panel)
	: d2d::OrthoCanvas(panel)
	, m_panel(panel)
	, m_timer(this, TIMER_ID)
{
	m_timer.Start(100);
}

void StageCanvas::onDraw()
{
	// todo easymap里双击打开绘制有问题，怀疑GL状态不对
	d2d::ShaderMgr::Instance()->sprite();

	m_panel->traverseSprites(d2d::DrawSpritesVisitor(), d2d::e_visible);
	m_panel->traverseShapes(d2d::DrawShapesVisitor(), d2d::e_visible);

	d2d::PrimitiveDraw::cross(d2d::Vector(0, 0), 100, 100, d2d::Colorf(1, 0, 0));

	m_editPanel->drawEditTemp();
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}

}