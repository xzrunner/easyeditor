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
	m_timer.Start(1000 / 30);
}

void StageCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();

	m_panel->Load();
	resetViewport();	// todo: resize -> new -> resize
}

void StageCanvas::onSize(int w, int h)
{
	d2d::OrthoCanvas::onSize(w, h);

	eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
	if (scr) {
		scr->OnSize(w, h);
	}
}

void StageCanvas::onDraw()
{
	m_panel->Update();

  	m_panel->traverseSprites(d2d::DrawSpritesVisitor(), d2d::e_visible);
  	m_panel->traverseShapes(d2d::DrawShapesVisitor(), d2d::e_visible);
  
  	m_editPanel->drawEditTemp();

	// turn to ejoy2d shader
	{
		// reset
		eejoy2d::EJShader::Reset();

		m_panel->Draw();

		assert(eejoy2d::EJScreen::Instance());
		eejoy2d::EJScreen::Instance()->DebugDraw();
	}
	// turn to easy2d shader
	{
		// reset
		d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
		shader->SetTexture(0);
		shader->null();

		DrawEJScreen();
	}
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

	eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
	assert(scr);

	int left = 0, right = scr->GetWidth();
	int down = 0, up = scr->GetHeight();

	vb[0] = left, vb[1] = down;
	vb[4] = left, vb[5] = up;
	vb[8] = right, vb[9] = up;
	vb[12] = right, vb[13] = down;

	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
 	shader->sprite();
 	shader->Draw(vb, scr->GetTexID());
}

}