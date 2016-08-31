#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Settings.h"

#include <ee/dev_config.h>
#include <ee/DrawSpritesVisitor.h>
#include <ee/SceneNode.h>
#include <ee/Config.h>
#include <ee/EE_DTex.h>
#include <ee/SpriteRenderer.h>
#include <ee/color_config.h>
#include <ee/CameraMgr.h>
#include <ee/cfg_const.h>

#include <easyanim.h>
#include <easytext.h>

#include <sprite2/S2_RVG.h>

namespace ecomplex
{

StageCanvas::StageCanvas(StagePanel* editPanel,
						 ee::LibraryPanel* library,
						 wxGLContext* glctx)
	: ee::CameraCanvas(editPanel, editPanel->GetStageImpl(), glctx)
	, m_stage(editPanel)
	, m_library(library)
	, m_background(NULL)
	, m_fps(1)
{
}

StageCanvas::~StageCanvas()
{
	if (m_background) {
		m_background->RemoveReference();
	}
}

void StageCanvas::SetBackground(ee::Symbol* sym)
{
	cu::RefCountObjAssign<ee::Symbol>(m_background, sym);
}

void StageCanvas::OnSize(int w, int h)
{
	ee::CameraCanvas::OnSize(w, h);

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

	DrawBackground();

	ee::ScreenCache::Instance()->Draw(ee::CameraMgr::Instance()->GetCamera());

	s2::RVG::SetColor(s2::Color(0, 0.8f, 0));
	const sm::rect& r = m_stage->GetSymbol()->GetScissor();
	s2::RVG::Rect(sm::vec2(r.xmin, r.ymin), sm::vec2(r.xmax, r.ymax), m_clipboxStyle.filling);

	if (Settings::bVisibleBGCross)
	{
		const float EDGE = 100;
		s2::RVG::SetColor(ee::LIGHT_GREY);
		s2::RVG::Cross(sm::vec2(0,0), EDGE, EDGE);
	}

	m_stage->DrawEditOP();

	m_fps.End();

#ifdef _DEBUG 
	if (ee::Config::Instance()->IsUseDTex()) {
		ee::DTex::Instance()->DebugDraw();
	}
#endif

	m_fps.DrawTime();

	wxLogDebug("++++++++ StageCanvas::OnDrawSprites end");
}

#else

void StageCanvas::OnDrawSprites() const
{
	m_fps.Begin();

	DrawBackground();

	float scale = ee::CameraMgr::Instance()->GetCamera()->GetScale();
	m_stage->TraverseSprites(ee::DrawSpritesVisitor(GetVisibleRegion(), scale), ee::DT_VISIBLE);

	const sm::rect& clipbox = dynamic_cast<const Symbol*>(m_stage->GetSymbol())->GetScissor();
	sm::vec2 sz = clipbox.Size();
	if (sz.x != 0 && sz.y != 0) {
		s2::RVG::SetColor(s2::Color(0, 204, 0));
		s2::RVG::LineWidth(2);
		s2::RVG::Rect(sm::vec2(clipbox.xmin, clipbox.ymin), sm::vec2(clipbox.xmax, clipbox.ymax), false);
	}

	if (Settings::bVisibleBGCross)
	{
		const float EDGE = 100;
		s2::RVG::SetColor(ee::LIGHT_GREY);
		s2::RVG::Cross(sm::vec2(0,0), EDGE, EDGE);
	}

	ee::SceneNodeMgr::Instance()->Draw();

	m_stage->DrawEditOP();

	m_fps.End();

#ifdef _DEBUG 
	if (ee::Config::Instance()->IsUseDTex()) {
		ee::DTex::Instance()->DebugDraw();
	}
#endif

	m_fps.DrawTime();
}

#endif // OPEN_SCREEN_CACHE

void StageCanvas::DrawBackground() const
{
	if (m_background) {
		ee::SpriteRenderer::Instance()->Draw(m_background);
	}

	if (Settings::bVisibleBGRect) {
		s2::RVG::SetColor(s2::Color(204, 204, 204));
		s2::RVG::LineWidth(2);
		s2::RVG::Rect(sm::vec2(0, 0), ee::HALF_SCREEN_WIDTH, ee::HALF_SCREEN_HEIGHT, false);
	}
}

}