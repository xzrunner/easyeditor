#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Settings.h"

#include <ee/dev_config.h>
#include <ee/DrawSpritesVisitor.h>
#include <ee/SceneNode.h>
#include <ee/Config.h>
#include <ee/EE_DTex.h>
#include <ee/EE_RVG.h>
#include <ee/SpriteRenderer.h>
#include <ee/color_config.h>

// debug
#include <ee/ShaderMgr.h>

#include <easyanim.h>
#include <easytext.h>

namespace ecomplex
{

StageCanvas::StageCanvas(StagePanel* editPanel,
						 ee::LibraryPanel* library,
						 wxGLContext* glctx)
	: ee::OrthoCanvas(editPanel, editPanel->GetStageImpl(), glctx)
	, m_stage(editPanel)
	, m_library(library)
	, m_background(NULL)
	, m_fps(1)
{
}

StageCanvas::~StageCanvas()
{
	if (m_background) {
		m_background->Release();
	}
}

void StageCanvas::SetBackground(ee::Symbol* symbol)
{
	ee::obj_assign<ee::Symbol>(m_background, symbol);
}

void StageCanvas::OnSize(int w, int h)
{
	ee::OrthoCanvas::OnSize(w, h);

// 	m_camera3.SetScreenSize(w, h);
// 
// 	e3d::ShaderMgr* shader_mgr = e3d::ShaderMgr::Instance();
// 	shader_mgr->SetProjection(w, h);
// 	shader_mgr->SetModelView(m_camera3.GetModelViewMat());
}

#ifdef OPEN_SCREEN_CACHE

void StageCanvas::OnDrawSprites() const
{
	wxLogDebug("++++++++ StageCanvas::OnDrawSprites begin");

	m_fps.Begin();

	drawBackground();

	ee::ScreenCache::Instance()->Draw(m_camera);

	ee::RVG::Color(ee::Colorf(0, 0.8f, 0));
	const ee::Rect& r = m_stage->getSymbol()->m_clipbox;
	ee::RVG::Rect(ee::Vector(r.xmin, r.ymin), ee::Vector(r.xmax, r.ymax), m_clipboxStyle.filling);

	if (Settings::bVisibleBGCross)
	{
		const float EDGE = 100;
		ee::RVG::Color(ee::LIGHT_GREY);
		ee::RVG::Cross(ee::Vector(0,0), EDGE, EDGE);
	}

	m_stage->DrawEditOP();

	m_fps.End();

#ifdef _DEBUG 
	if (ee::Config::Instance()->IsUseDTex()) {
		ee::DTex::Instance()->DebugDraw();
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

	m_stage->TraverseSprites(ee::DrawSpritesVisitor(m_screen.GetRegion(), m_camera->GetScale()), 
		ee::DT_VISIBLE);

	const ee::Rect& clipbox = m_stage->getSymbol()->m_clipbox;
	if (clipbox.Width() != 0 && clipbox.Height() != 0) {
		ee::RVG::Color(ee::Colorf(0, 0.8f, 0));
		ee::RVG::Rect(ee::Vector(clipbox.xmin, clipbox.ymin), ee::Vector(clipbox.xmax, clipbox.ymax), false);
	}

	if (Settings::bVisibleBGCross)
	{
		const float EDGE = 100;
		ee::RVG::Color(ee::LIGHT_GREY);
		ee::RVG::Cross(ee::Vector(0,0), EDGE, EDGE);
	}

	ee::SceneNodeMgr::Instance()->Draw();

	m_stage->DrawEditOP();

	m_fps.End();

// #ifdef _DEBUG 
// 	if (ee::Config::Instance()->IsUseDTex()) {
// 		ee::DTex::Instance()->DebugDraw();
// 	}
// #endif

	ee::ShaderMgr::Instance()->sprite();

	m_fps.DrawTime(m_screen);
}

#endif // OPEN_SCREEN_CACHE

void StageCanvas::drawBackground() const
{
	if (m_background) {
		ee::SpriteRenderer::Instance()->Draw(m_background);
	}

	if (Settings::bVisibleBGRect) {
		ee::RVG::Color(ee::Colorf(0.8f, 0.8f, 0.8f));
		ee::RVG::Rect(ee::Vector(0, 0), 1024 * 0.5f, 768 * 0.5f, false);
	}
}

}