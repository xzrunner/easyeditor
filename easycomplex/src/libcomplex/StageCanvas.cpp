#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Settings.h"

#include <easyanim.h>

namespace ecomplex
{

StageCanvas::StageCanvas(StagePanel* editPanel)
	: d2d::DynamicStageCanvas(editPanel)
	, m_stage(editPanel)
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
	d2d::obj_assign((d2d::Object*&)m_background, symbol);
}

void StageCanvas::InitGL()
{
	d2d::DynamicStageCanvas::InitGL();
	m_stage->getSymbol()->ReloadTexture();
// 		d2d::DynamicTexture::Instance()->ReloadTexture();
// 		d2d::DynamicFont::Instance()->ReloadTexture();

	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->ReloadTexture();
	}

	ResetViewport();
}

void StageCanvas::OnDrawSprites() const
{
	m_stat.Begin();

	drawBackground();

	d2d::ShaderMgr* shader_mgr = d2d::ShaderMgr::Instance();
	m_stage->traverseSprites(d2d::DrawSpritesVisitor(m_screen.GetRegion(), m_camera->GetScale()), 
		d2d::DT_VISIBLE);
	shader_mgr->SetSpriteShader(0);
	shader_mgr->sprite();

	d2d::PrimitiveDraw::rect(m_stage->getSymbol()->m_clipbox, m_clipboxStyle);

	if (Settings::bVisibleBGCross)
	{
		const float EDGE = 100;
		d2d::PrimitiveDraw::cross(d2d::Vector(0,0), EDGE, EDGE, d2d::LIGHT_GREY);
	}

 	m_stage->DrawEditOP();

	m_stat.End();

#ifdef _DEBUG 
//		d2d::DynamicTexture::Instance()->DebugDraw();
//		d2d::DynamicFont::Instance()->DebugDraw();
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->DebugDraw();
	}
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