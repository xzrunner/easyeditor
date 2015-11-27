#include "TwoPassCanvas.h"

#include "common/Config.h"
#include "common/color_config.h"
#include "render/ShaderMgr.h"
#include "render/ScreenFBO.h"
#include "render/SpriteRenderer.h"
#include "render/RenderContext.h"
#include "render/DrawCallBatching.h"

#include <dtex_facade.h>

namespace d2d
{

TwoPassCanvas::TwoPassCanvas(wxWindow* stage_wnd, EditPanelImpl* stage)
	: IStageCanvas(stage_wnd, stage)
{
	m_scr_style.multi_col = WHITE;
	m_scr_style.add_col = BLACK;
}

void TwoPassCanvas::InitGL()
{
	// fix me: init render context
	ShaderMgr::Instance();

	IStageCanvas::InitGL();

	dtexf_cs_reload();
}

void TwoPassCanvas::OnSize(int w, int h)
{
	if (Config::Instance()->IsUseDTex()) {
		DrawCallBatching::Instance()->OnSize(w, h);
	}
}

// void TwoPassCanvas::OnDrawWhole() const
// {
// 	const FBO& fbo = ScreenFBO::Instance()->GetFBO();
// 	ShaderMgr* mgr = ShaderMgr::Instance();
// 
// 	SpriteRenderer::Instance()->SetCamera(GetCamera());
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	// Draw to FBO
// 	//////////////////////////////////////////////////////////////////////////
// 	if (IsDirty()) {
//  		mgr->SetFBO(fbo.GetFboID());
// 		
// 		glClearColor(0, 0, 0, 0);
// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 
// 		OnDrawSprites();
// 
// 		RenderContext::Flush();
// 
// //		wxLogDebug("pass 22222222222222222");
// 	} else {
// //		wxLogDebug("pass 1");
// 	}
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	// Draw to Screen
// 	//////////////////////////////////////////////////////////////////////////
// 
// 	mgr->SetFBO(0);
// 	mgr->SetTexture(0);
// 
// 	glClearColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 
// 	mgr->Screen();
// 	mgr->DrawScreen(fbo.GetTexID());
// }

static void
_before_draw(void* ud) {
	TwoPassCanvas::ScreenStyle* stype = (TwoPassCanvas::ScreenStyle*)ud;
	ShaderMgr::Instance()->SetSpriteColor(stype->multi_col, stype->add_col);
}

void TwoPassCanvas::OnDrawWhole() const
{
	SpriteRenderer::Instance()->SetCamera(GetCamera());

	if (dtexf_cs_open()) 
	{
		//////////////////////////////////////////////////////////////////////////
		// Draw to Target
		//////////////////////////////////////////////////////////////////////////
		if (IsDirty()) {
			dtexf_cs_bind();

			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			OnDrawSprites();

			dtexf_cs_unbind();
		}

		//////////////////////////////////////////////////////////////////////////
		// Draw to Screen
		//////////////////////////////////////////////////////////////////////////
		glClearColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		dtexf_cs_draw_to_screen(&_before_draw, (ScreenStyle*)(&m_scr_style));
	} 
	else 
	{
		glClearColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		OnDrawSprites();

		RenderContext::Flush();
	}
}

}