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
	m_panel->Update();

  	m_panel->traverseSprites(d2d::DrawSpritesVisitor(), d2d::e_visible);
  	m_panel->traverseShapes(d2d::DrawShapesVisitor(), d2d::e_visible);
  
  	m_editPanel->drawEditTemp();

	// turn to ejoy2d shader

	m_panel->Draw();

	// turn to easy2d shader

	DrawEJScreen();
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}

void StageCanvas::DrawEJScreen() const
{
	float vb[16];

	vb[2] = 0, vb[3] = 0;
	vb[6] = 0, vb[7] = 1;
	vb[10] = 1, vb[11] = 1;
	vb[14] = 1, vb[15] = 0;

 	int left = -200, right = 200;
 	int down = -150, up = 150;

	vb[0] = left, vb[1] = down;
	vb[4] = left, vb[5] = up;
	vb[8] = right, vb[9] = up;
	vb[12] = right, vb[13] = down;

	GLuint tex = eejoy2d::EJScreen::Instance()->GetTexID();

	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	// reset
	shader->null();
 	shader->sprite();
 	shader->Draw(vb, tex);
}

}