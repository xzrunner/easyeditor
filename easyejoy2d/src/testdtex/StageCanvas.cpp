#include "StageCanvas.h"
#include "StagePanel.h"

#include <easyejoy2d.h>

namespace tdtex
{

StageCanvas::StageCanvas(StagePanel* panel)
	: d2d::DynamicStageCanvas(panel)
	, m_panel(panel)
{
}

void StageCanvas::InitGL()
{
	d2d::DynamicStageCanvas::InitGL();

	m_panel->Load();
	ResetViewport();	// todo: resize -> new -> resize
}

void StageCanvas::OnSize(int w, int h)
{
	d2d::DynamicStageCanvas::OnSize(w, h);

	eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
	if (scr) {
		scr->OnSize(w, h);
	}
}

void StageCanvas::OnDrawSprites() const
{
	m_panel->Update();

	d2d::Rect sr = m_screen.GetRegion();
  	m_panel->TraverseSprites(d2d::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		d2d::DT_VISIBLE);
//  	m_panel->traverseShapes(d2d::DrawShapesVisitor(sr), d2d::DT_VISIBLE);
  
  	m_stage->DrawEditOP();

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