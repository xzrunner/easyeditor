#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Settings.h"

#include <easyanim.h>
#include <easytext.h>

namespace ecomplex
{

StageCanvas::StageCanvas(StagePanel* editPanel,
						 d2d::LibraryPanel* library)
	: d2d::OrthoCanvas(editPanel, editPanel->GetStageImpl())
	, m_stage(editPanel)
	, m_library(library)
	, m_background(NULL)
	, m_stat(1)
{
	m_bgStyle.color.set(0.8f, 0.8f, 0.8f);
	m_clipboxStyle.color.set(0, 0.8f, 0);
}

StageCanvas::~StageCanvas()
{
	if (m_background) {
		m_background->Release();
	}
}

void StageCanvas::SetBackground(d2d::ISymbol* symbol)
{
	d2d::obj_assign<d2d::ISymbol>(m_background, symbol);
}

void StageCanvas::InitGL()
{
	d2d::ShaderMgr::Instance();

	d2d::OrthoCanvas::InitGL();

	m_library->LoadDefaultSymbol();

	e3d::ShaderMgr* shader_mgr = e3d::ShaderMgr::Instance();
	shader_mgr->Null();
	shader_mgr->SetModelView(m_camera3.GetModelViewMat());

 	if (d2d::Config::Instance()->IsUseDTex()) {
 		d2d::DrawCallBatching::Instance()->ReloadBegin();
 	}
	m_stage->getSymbol()->ReloadTexture();
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DrawCallBatching::Instance()->ReloadEnd();
	}

	ResetViewport();
}

void StageCanvas::OnSize(int w, int h)
{
	d2d::OrthoCanvas::OnSize(w, h);

	m_camera3.SetScreenSize(w, h);

	e3d::ShaderMgr* shader_mgr = e3d::ShaderMgr::Instance();
	shader_mgr->SetProjection(w, h);
	shader_mgr->SetModelView(m_camera3.GetModelViewMat());
}

void StageCanvas::OnDrawSprites() const
{
	m_stat.Begin();

	drawBackground();

	d2d::ShaderMgr* shader_mgr = d2d::ShaderMgr::Instance();
	m_stage->TraverseSprites(d2d::DrawSpritesVisitor(m_screen.GetRegion(), m_camera->GetScale()), 
		d2d::DT_VISIBLE);

	d2d::PrimitiveDraw::rect(m_stage->getSymbol()->m_clipbox, m_clipboxStyle);

	if (Settings::bVisibleBGCross)
	{
		const float EDGE = 100;
		d2d::PrimitiveDraw::cross(d2d::Vector(0,0), EDGE, EDGE, d2d::LIGHT_GREY);
	}

 	m_stage->DrawEditOP();

	m_stat.End();

#ifdef _DEBUG 
// 	if (d2d::Config::Instance()->IsUseDTex()) {
// 		d2d::DrawCallBatching::Instance()->DebugDraw();
// 	}
#endif

	m_stat.DrawTime(m_screen);
}

void StageCanvas::drawBackground() const
{
	if (m_background) {
		d2d::SpriteRenderer::Instance()->Draw(m_background);
	}

	if (Settings::bVisibleBGRect) {
		d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), 1024 * 0.5f, 768 * 0.5f, m_bgStyle);
	}
}

}