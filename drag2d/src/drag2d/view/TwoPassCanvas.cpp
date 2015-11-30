#include "common/dev_config.h"

#include "TwoPassCanvas.h"

#include "common/Config.h"
#include "common/color_config.h"
#include "render/ShaderMgr.h"
#include "render/ScreenFBO.h"
#include "render/SpriteRenderer.h"
#include "render/RenderContext.h"
#include "render/DrawCallBatching.h"
#include "render/SpatialIndex.h"
#include "render/ScreenCache.h"
#include "view/Camera.h"

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

	ScreenCache::Instance()->Reload();
}

void TwoPassCanvas::OnSize(int w, int h)
{
	if (Config::Instance()->IsUseDTex()) {
		DrawCallBatching::Instance()->OnSize(w, h);
		ScreenCache::Instance()->OnSize(w, h);
	}
}

static void
_before_draw(void* ud) {
	TwoPassCanvas::ScreenStyle* stype = (TwoPassCanvas::ScreenStyle*)ud;
	ShaderMgr::Instance()->SetSpriteColor(stype->multi_col, stype->add_col);
}

#ifdef OPEN_SCREEN_CACHE

void TwoPassCanvas::OnDrawWhole() const
{
	SpriteRenderer::Instance()->SetCamera(GetCamera());

	ScreenCache* sc = ScreenCache::Instance();

	//////////////////////////////////////////////////////////////////////////
	// 1. Compute Invalid Rect
	// 2. Draw to Target
	//////////////////////////////////////////////////////////////////////////
	sc->Bind();

// 	glClearColor(0, 0, 0, 0);
// 	glClear(GL_COLOR_BUFFER_BIT);

	OnDrawSprites();

	SpatialIndex::Instance()->DebugDraw();

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
	SpriteRenderer::Instance()->SetCamera(GetCamera());

// 	if (ScreenCache::IsOpen()) 
// 	{
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
// 	} 
// 	else 
// 	{
// 		glClearColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
// 		glClear(GL_COLOR_BUFFER_BIT);
// 
// 		OnDrawSprites();
// 
// 		RenderContext::Flush();
// 	}
}

#endif // OPEN_SCREEN_CACHE

}