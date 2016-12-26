#include "dev_config.h"

#include "TwoPassCanvas.h"
#include "Config.h"
#include "color_config.h"
#include "SpriteRenderer.h"
#include "EE_SP.h"
#include "Camera.h"

#include <unirender/RenderTarget.h>
#include <unirender/RenderContext.h>
#include <unirender/Texture.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/FilterShader.h>
#include <shaderlab/ColGradingProg.h>
#include <shaderlab/Sprite2Shader.h>
#include <gum/RenderContext.h>
#include <gum/RenderTarget.h>

namespace ee
{

TwoPassCanvas::TwoPassCanvas(wxWindow* stage_wnd, EditPanelImpl* stage,
							 wxGLContext* glctx, bool use_context_stack)
	: StageCanvas(stage_wnd, stage, glctx, use_context_stack)
{
}

void TwoPassCanvas::OnSize(int w, int h)
{
	gum::RenderContext::Instance()->OnSize(w, h);
}

static void
_before_draw(void* ud) {
// 	if (ScreenCache::Instance()->IsColGradingEnable()) {
// 		sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
// 		sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
// 		sl::ColGradingProg* prog = static_cast<sl::ColGradingProg*>(shader->GetProgram(sl::FM_COL_GRADING));
// 		if (prog->IsTexValid()) {
// 			mgr->SetShader(sl::FILTER);
// 			shader->SetMode(sl::FM_COL_GRADING);
// 			return;
// 		}
// 	}

 	TwoPassCanvas::ScreenStyle* stype = (TwoPassCanvas::ScreenStyle*)ud;
 	s2::RenderColor color;
 	color.mul = stype->multi_col;
 	color.add = stype->add_col;
 	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
 	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
 	shader->SetColor(color.mul.ToABGR(), color.add.ToABGR());
 	shader->SetColorMap(color.rmap.ToABGR(), color.gmap.ToABGR(), color.bmap.ToABGR());
}

#ifdef OPEN_SCREEN_CACHE

void TwoPassCanvas::OnDrawWhole() const
{
	ScreenCache* sc = ScreenCache::Instance();

	//////////////////////////////////////////////////////////////////////////
	// 1. Compute Invalid sm::rect
	// 2. Draw to Target
	//////////////////////////////////////////////////////////////////////////
	sc->Bind();

// 	glClearColor(0, 0, 0, 0);
// 	glClear(GL_COLOR_BUFFER_BIT);

	OnDrawSprites();

	SpatialPartition::Instance()->DebugDraw();

	sc->Unbind();

	//////////////////////////////////////////////////////////////////////////
	// Draw to Screen
	//////////////////////////////////////////////////////////////////////////

	glClearColor(m_bg_color.r / 255.0f, m_bg_color.g / 255.0f, m_bg_color.b / 255.0f, m_bg_color.a / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	sc->DrawToScreen(&_before_draw, (ScreenStyle*)(&m_scr_style));
}

#else

void TwoPassCanvas::OnDrawWhole() const
{
	ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();

 	//if (ScreenCache::IsOpen()) 
 	{
		//////////////////////////////////////////////////////////////////////////
		// Draw to Target
		//////////////////////////////////////////////////////////////////////////
		if (IsDirty()) {
			ur::RenderTarget* rt = gum::RenderTarget::Instance()->GetScreen0();
			rt->Bind();

			rc->SetClearFlag(ur::MASKC);
			rc->Clear(0);

			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			OnDrawSprites();

			rt->Unbind();
		}

		//////////////////////////////////////////////////////////////////////////
		// Draw to Screen
		//////////////////////////////////////////////////////////////////////////

		rc->SetClearFlag(m_bg_color.a << 24 | m_bg_color.r << 16 | m_bg_color.g << 8 | m_bg_color.b);
		rc->Clear(0);

		sm::vec2 vertices[4], texcoords[4];
		vertices[0].Set(-1, -1);
		vertices[1].Set( 1, -1);
		vertices[2].Set( 1,  1);
		vertices[3].Set(-1,  1);
		texcoords[0].Set(0, 0);
		texcoords[1].Set(1, 0);
		texcoords[2].Set(1, 1);
		texcoords[3].Set(0, 1);

		ur::RenderTarget* rt = gum::RenderTarget::Instance()->GetScreen0();
		sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
		mgr->SetShader(sl::SPRITE2);
		sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
		shader->Draw(&vertices[0].x, &texcoords[0].x, rt->GetTexture()->ID());
 	} 
 	//else 
 	//{
 	//	glClearColor(m_bg_color.r / 255.0f, m_bg_color.g / 255.0f, m_bg_color.b / 255.0f, m_bg_color.a / 255.0f);
 	//	glClear(GL_COLOR_BUFFER_BIT);
 
 	//	OnDrawSprites();
 
 	//	ShaderContext::Flush();
 	//}
}

#endif // OPEN_SCREEN_CACHE

}