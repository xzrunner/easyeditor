#include "dev_config.h"

#include "TwoPassCanvas.h"
#include "Config.h"
#include "color_config.h"
#include "SpriteRenderer.h"
#include "EE_SP.h"

#include <unirender/UR_RenderTarget.h>
#include <unirender/UR_RenderContext.h>
#include <unirender/UR_Texture.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/FilterShader.h>
#include <shaderlab/ColGradingProg.h>
#include <shaderlab/Sprite2Shader.h>
#include <sprite2/RenderCtxStack.h>
#include <sprite2/Blackboard.h>
#include <gum/RenderContext.h>
#include <gum/RenderTarget.h>
#include <gum/RenderTargetMgr.h>
#include <gum/GUM_DTex.h>
#include <gum/GUM_Sprite2.h>
#include <gum/GUM_DRect.h>

namespace ee
{

TwoPassCanvas::TwoPassCanvas(wxWindow* stage_wnd, EditPanelImpl* stage,
							 wxGLContext* glctx, bool use_context_stack)
	: StageCanvas(stage_wnd, stage, glctx, use_context_stack)
	, m_rt(NULL)
{
}

TwoPassCanvas::~TwoPassCanvas()
{
	gum::RenderTargetMgr::Instance()->Return(m_rt);
}

void TwoPassCanvas::OnSize(int w, int h)
{
	gum::RenderContext::Instance()->OnSize(w, h);

	gum::RenderTargetMgr* rt_mgr = gum::RenderTargetMgr::Instance();
	rt_mgr->OnSize(w, h);
	if (!m_rt) {
		m_rt = rt_mgr->Fetch();
		assert(m_rt);
	}

	s2::Blackboard::Instance()->SetScreenCacheTexID(m_rt->GetTexID());
}

static void
_before_draw(void* ud) {

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
//	DrawOnePass();

	DrawTwoPass();

	if (Config::Instance()->IsDebugDrawOpen()) {
		DebugDraw();
	}
}

#endif // OPEN_SCREEN_CACHE

void TwoPassCanvas::DrawOnePass() const
{
	ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();
	rc->SetClearFlag(ur::MASKC);
	rc->Clear(m_bg_color.a << 24 | m_bg_color.r << 16 | m_bg_color.g << 8 | m_bg_color.b);

	OnDrawSprites();

	sl::ShaderMgr::Instance()->FlushShader();
}

void TwoPassCanvas::DrawTwoPass() const
{
	//////////////////////////////////////////////////////////////////////////
	// Draw to Target
	//////////////////////////////////////////////////////////////////////////
	if (IsDirty()) {
		DrawPass1();
	}

	//////////////////////////////////////////////////////////////////////////
	// Draw to Screen
	//////////////////////////////////////////////////////////////////////////

	s2::RenderCtxStack::Instance()->Push(s2::RenderContext(2, 2, 0, 0));

	ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();
	rc->SetClearFlag(ur::MASKC);
	rc->Clear(m_bg_color.a << 24 | m_bg_color.r << 16 | m_bg_color.g << 8 | m_bg_color.b);

	sm::vec2 vertices[4], texcoords[4];
 	vertices[0].Set(-1, -1);
 	vertices[1].Set( 1, -1);
 	vertices[2].Set( 1,  1);
 	vertices[3].Set(-1,  1);
	texcoords[0].Set(0, 0);
	texcoords[1].Set(1, 0);
	texcoords[2].Set(1, 1);
	texcoords[3].Set(0, 1);

	DrawPass2(&vertices[0].x, &texcoords[0].x, m_rt->GetTexID());
 	sl::ShaderMgr::Instance()->FlushShader();

	s2::RenderCtxStack::Instance()->Pop();
}

void TwoPassCanvas::DrawPass1() const
{
	ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();

	m_rt->Bind();

	rc->SetClearFlag(ur::MASKC);
	rc->Clear(0);

	DrawDirect();
//	DrawDRect();

	m_rt->Unbind();
}

void TwoPassCanvas::DrawPass2(const float* vertices, const float* texcoords, int tex_id) const
{
	if (m_scr_style.col_grading) 
	{
		sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
		sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
		sl::ColGradingProg* prog = static_cast<sl::ColGradingProg*>(shader->GetProgram(sl::FM_COL_GRADING));
		if (prog->IsTexValid()) {
			mgr->SetShader(sl::FILTER);
			shader->SetMode(sl::FM_COL_GRADING);
			shader->Draw(vertices, texcoords, tex_id);
			return;
		}
	}

	s2::RenderColor color;
	color.SetMul(m_scr_style.multi_col);
	color.SetAdd(m_scr_style.add_col);
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->SetColor(color.GetMulABGR(), color.GetAddABGR());
	shader->SetColorMap(color.GetRMapABGR(), color.GetGMapABGR(), color.GetBMapABGR());
	shader->DrawQuad(vertices, texcoords, tex_id);
}

void TwoPassCanvas::DrawDirect() const
{
	OnDrawSprites();
}

void TwoPassCanvas::DrawDRect() const
{
// 	ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();
// 
// 	sm::vec2 offset = s2::RenderCtxStack::Instance()->Top()->GetMVOffset();
// 	float scale = s2::RenderCtxStack::Instance()->Top()->GetMVScale();
// 
// 	gum::DRect* drect = gum::DRect::Instance();
// 	if (!drect->Draw(offset , scale))
// 	{
// 		drect->Bind();
//  		rc->SetClearFlag(ur::MASKC);
//  		rc->Clear(0xffff00ff);
//  		OnDrawSprites();
// 		drect->Unbind();
// 	}
}

void TwoPassCanvas::DebugDraw() const
{
 	gum::DTex::Instance()->DebugDraw();
// 	gum::Sprite2::Instance()->DebugDraw();
//	gum::DRect::Instance()->DebugDraw();

//	gum::RenderTargetMgr::Instance()->DebugDraw();
}

}