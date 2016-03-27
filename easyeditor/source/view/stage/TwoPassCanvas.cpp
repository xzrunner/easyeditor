#include "dev_config.h"

#include "TwoPassCanvas.h"
#include "Config.h"
#include "color_config.h"
#include "ShaderMgr.h"
#include "SpriteShader.h"
#include "SpriteRenderer.h"
#include "ShaderContext.h"
#include "EE_DTex.h"
#include "EE_SP.h"
#include "ScreenCache.h"
#include "Camera.h"
#include "BlendShader.h"

#include <shaderlab.h>

namespace ee
{

TwoPassCanvas::TwoPassCanvas(wxWindow* stage_wnd, EditPanelImpl* stage,
							 wxGLContext* glctx, bool use_context_stack)
	: StageCanvas(stage_wnd, stage, glctx, use_context_stack)
{
	m_scr_style.multi_col = WHITE;
	m_scr_style.add_col = BLACK;
}

void TwoPassCanvas::OnSize(int w, int h)
{
	if (Config::Instance()->IsUseDTex()) {
		DTex::Instance()->OnSize(w, h);
	} else {
		// fix me: ScreenCache depend on DTex
		DTex::Instance();
	}
	ScreenCache::Instance()->SetSize(w, h);

	sl_blend_on_size(w, h);
	sl_filter_on_size(w, h);
}

static void
_before_draw(void* ud) {
	TwoPassCanvas::ScreenStyle* stype = (TwoPassCanvas::ScreenStyle*)ud;
	ColorTrans color;
	color.multi = stype->multi_col;
	color.add = stype->add_col;
	ShaderMgr* mgr = ShaderMgr::Instance();
	SpriteShader* shader = static_cast<SpriteShader*>(mgr->GetShader(ShaderMgr::SPRITE));
	shader->SetColor(color);
}

#ifdef OPEN_SCREEN_CACHE

void TwoPassCanvas::OnDrawWhole() const
{
	ScreenCache* sc = ScreenCache::Instance();

	//////////////////////////////////////////////////////////////////////////
	// 1. Compute Invalid Rect
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

	glClearColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
	glClear(GL_COLOR_BUFFER_BIT);

	sc->DrawToScreen(&_before_draw, (ScreenStyle*)(&m_scr_style));
}

#else

void TwoPassCanvas::OnDrawWhole() const
{
 	//if (ScreenCache::IsOpen()) 
 	{
		//////////////////////////////////////////////////////////////////////////
		// Draw to Target
		//////////////////////////////////////////////////////////////////////////
		if (IsDirty()) {
			ScreenCache::Instance()->Bind();

			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			OnDrawSprites();

			ScreenCache::Instance()->Unbind();
		}

		//////////////////////////////////////////////////////////////////////////
		// Draw to Screen
		//////////////////////////////////////////////////////////////////////////
		glClearColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
		glClear(GL_COLOR_BUFFER_BIT);

		ScreenCache::Instance()->DrawToScreen(&_before_draw, (ScreenStyle*)(&m_scr_style));
 	} 
 	//else 
 	//{
 	//	glClearColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
 	//	glClear(GL_COLOR_BUFFER_BIT);
 
 	//	OnDrawSprites();
 
 	//	ShaderContext::Flush();
 	//}
}

#endif // OPEN_SCREEN_CACHE

}