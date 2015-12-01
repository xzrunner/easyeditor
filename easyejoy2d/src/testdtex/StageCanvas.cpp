#include "StageCanvas.h"
#include "StagePanel.h"

#include <easyejoy2d.h>
#include <dtex.h>

namespace tdtex
{

StageCanvas::StageCanvas(StagePanel* panel)
	: d2d::OnePassCanvas(panel, panel->GetStageImpl())
	, m_panel(panel)
{
}

void StageCanvas::InitGL()
{
	d2d::OnePassCanvas::InitGL();

	m_panel->Load();

	// todo: resize -> new -> resize
	d2d::ResetViewportSJ::Instance()->Reset();
}

void StageCanvas::OnSize(int w, int h)
{
	//////////////////////////////////////////////////////////////////////////
	// copy from OnePassCanvas
	//////////////////////////////////////////////////////////////////////////
	// Makes the OpenGL state that is represented by the OpenGL rendering 
	// context context current
	if (IsInited()) {
		SetCurrentCanvas();
	}

	glViewport(0, 0, w, h);

	m_screen.SetSize(w, h);
	m_screen.SetCamera();

	m_camera->UpdateModelView();

	d2d::RenderContext::GetCurrContext()->SetProjection(w, h);
	//////////////////////////////////////////////////////////////////////////

	eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
	scr->OnSize(w, h);
}

void StageCanvas::OnDrawSprites() const
{
	d2d::Rect sr = m_screen.GetRegion();
  	m_panel->TraverseSprites(d2d::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		d2d::DT_VISIBLE);
//  	m_panel->traverseShapes(d2d::DrawShapesVisitor(sr), d2d::DT_VISIBLE);
  
  	m_stage->DrawEditOP();

	// turn to ejoy2d shader
	{
		// reset
		eejoy2d::EJShader::Reset();
		m_panel->Update();	// after EJShader::Reset()

		dtex_stat_draw_start();
		m_panel->Draw();
		dtex_stat_draw_end();

 		assert(eejoy2d::EJScreen::Instance());

		eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
		assert(scr);
		scr->Bind();
 		eejoy2d::EJScreen::Instance()->DebugDraw();
		scr->UnBind();
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