#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/Camera.h>
#include <ee/RenderContextStack.h>
#include <ee/DrawSpritesVisitor.h>
#include <ee/ShaderMgr.h>
#include <ee/EditPanelImpl.h>

#include <easyejoy2d.h>
#include <dtex.h>

namespace tdtex
{

StageCanvas::StageCanvas(StagePanel* panel)
	: ee::OnePassCanvas(panel, panel->GetStageImpl())
	, m_panel(panel)
{
	m_panel->Load();
}

void StageCanvas::OnSize(int w, int h)
{
	//////////////////////////////////////////////////////////////////////////
	// copy from OnePassCanvas
	//////////////////////////////////////////////////////////////////////////
	glViewport(0, 0, w, h);

	m_screen.SetSize(w, h);
	m_screen.SetCamera();

	m_camera->UpdateModelView();

	ee::RenderContextStack::Instance()->SetProjection(w, h);
	//////////////////////////////////////////////////////////////////////////

	eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
	scr->OnSize(w, h);
}

void StageCanvas::OnDrawSprites() const
{
	ee::Rect sr = m_screen.GetRegion();
  	m_panel->TraverseSprites(ee::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		ee::DT_VISIBLE);
//  	m_panel->traverseShapes(ee::DrawShapesVisitor(sr), ee::DT_VISIBLE);
  
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
		ee::ShaderMgr* shader = ee::ShaderMgr::Instance();
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

	ee::ShaderMgr* shader = ee::ShaderMgr::Instance();
 	shader->sprite();
 	shader->Draw(vb, scr->GetTexID());
}

}