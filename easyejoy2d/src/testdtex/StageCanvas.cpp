#include "StageCanvas.h"
#include "StagePanel.h"

#include <easyejoy2d.h>

namespace tdtex
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

void StageCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();

	m_panel->Load();
}

void StageCanvas::onDraw()
{
  	m_panel->traverseSprites(d2d::DrawSpritesVisitor(), d2d::e_visible);
  	m_panel->traverseShapes(d2d::DrawShapesVisitor(), d2d::e_visible);
  
  	m_editPanel->drawEditTemp();

 	m_panel->Draw();
// 	DrawEJScreen();
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}

void StageCanvas::DrawEJScreen() const
{
	float vb[16];

	vb[0] = 0, vb[1] = 0;
	vb[2] = 0, vb[3] = 0;

	vb[4] = 0, vb[5] = 600;
	vb[6] = 0, vb[7] = 1;

	vb[8] = 800, vb[9] = 600;
	vb[10] = 1, vb[11] = 1;

	vb[12] = 800, vb[13] = 0;
	vb[14] = 1, vb[15] = 0;

	GLuint tex = eejoy2d::EJScreen::Instance()->GetTexID();

	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	shader->sprite();
	shader->Draw(vb, tex);
	shader->Flush();
}

}