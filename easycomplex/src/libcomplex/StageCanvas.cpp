#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Settings.h"

#include <easyanim.h>
#include <easytext.h>

// todo
#include <common/dev_config.h>

namespace ecomplex
{

StageCanvas::StageCanvas(StagePanel* editPanel,
						 d2d::LibraryPanel* library,
						 wxGLContext* glctx)
	: d2d::OrthoCanvas(editPanel, editPanel->GetStageImpl(), glctx)
	, m_stage(editPanel)
	, m_library(library)
	, m_background(NULL)
	, m_fps(1)
{
	m_bgStyle.color.Set(0.8f, 0.8f, 0.8f);
	m_clipboxStyle.color.Set(0, 0.8f, 0);
}

StageCanvas::~StageCanvas()
{
	if (m_background) {
		m_background->Release();
	}
}

void StageCanvas::SetBackground(d2d::Symbol* symbol)
{
	d2d::obj_assign<d2d::Symbol>(m_background, symbol);
}

void StageCanvas::OnSize(int w, int h)
{
	d2d::OrthoCanvas::OnSize(w, h);

	m_camera3.SetScreenSize(w, h);

	e3d::ShaderMgr* shader_mgr = e3d::ShaderMgr::Instance();
	shader_mgr->SetProjection(w, h);
	shader_mgr->SetModelView(m_camera3.GetModelViewMat());
}

#ifdef OPEN_SCREEN_CACHE

void StageCanvas::OnDrawSprites() const
{
	wxLogDebug("++++++++ StageCanvas::OnDrawSprites begin");

	m_fps.Begin();

	drawBackground();

	d2d::ScreenCache::Instance()->Draw(m_camera);

	d2d::PrimitiveDraw::DrawRect(m_stage->getSymbol()->m_clipbox, m_clipboxStyle);

	if (Settings::bVisibleBGCross)
	{
		const float EDGE = 100;
		d2d::PrimitiveDraw::Cross(d2d::Vector(0,0), EDGE, EDGE, d2d::LIGHT_GREY);
	}

	m_stage->DrawEditOP();

	m_fps.End();

#ifdef _DEBUG 
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DrawCallBatching::Instance()->DebugDraw();
	}
#endif

	m_fps.DrawTime(m_screen);

	wxLogDebug("++++++++ StageCanvas::OnDrawSprites end");
}

#else

void StageCanvas::OnDrawSprites() const
{
	m_fps.Begin();

	drawBackground();

	m_stage->TraverseSprites(d2d::DrawSpritesVisitor(m_screen.GetRegion(), m_camera->GetScale()), 
		d2d::DT_VISIBLE);

	d2d::PrimitiveDraw::DrawRect(m_stage->getSymbol()->m_clipbox, m_clipboxStyle);

	if (Settings::bVisibleBGCross)
	{
		const float EDGE = 100;
		d2d::PrimitiveDraw::Cross(d2d::Vector(0,0), EDGE, EDGE, d2d::LIGHT_GREY);
	}

	d2d::SceneNodeMgr::Instance()->Draw();

	m_stage->DrawEditOP();

	m_fps.End();

#ifdef _DEBUG 
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DrawCallBatching::Instance()->DebugDraw();
	}
#endif

	m_fps.DrawTime(m_screen);
}

#endif // OPEN_SCREEN_CACHE

void StageCanvas::drawBackground() const
{
	if (m_background) {
		d2d::SpriteRenderer::Instance()->Draw(m_background);
	}

	if (Settings::bVisibleBGRect) {
		d2d::PrimitiveDraw::DrawRect(d2d::Vector(0, 0), 1024 * 0.5f, 768 * 0.5f, m_bgStyle);
	}
}

}