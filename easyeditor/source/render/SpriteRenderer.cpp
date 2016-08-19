#include "SpriteRenderer.h"
#include "Sprite.h"
#include "Symbol.h"
#include "Math2D.h"
#include "color_config.h"
#include "CameraMgr.h"
#include "Camera.h"
#include "Config.h"
#include "SettingData.h"
#include "SpriteGaussianBlur.h"
#include "SpriteOuterGlow.h"

#include <sprite2/RenderFilter.h>
#include <sprite2/RFEdgeDetection.h>
#include <sprite2/RFGaussianBlur.h>
#include <sprite2/RFOuterGlow.h>
#include <sprite2/S2_RVG.h>
#include <sprite2/BoundingBox.h>
#include <sprite2/DrawBlend.h>

#include <shaderlab.h>

namespace ee
{

void SpriteRenderer::Draw(const Sprite* spr, 
						  const s2::RenderParams& params)
{
	if (!spr->IsVisible()) {
		return;
	}

	s2::RenderShader rs = spr->Shader() * params.shader;
	s2::RenderCamera rc = spr->Camera() * params.camera;

	sl::RenderContext* ctx = sl::ShaderMgr::Instance()->GetContext();
	switch (rs.fast_blend)
	{
	case s2::FBM_NULL:
		ctx->SetBlend(2, 6);		// BLEND_GL_ONE, BLEND_GL_ONE_MINUS_SRC_ALPHA
		ctx->SetBlendEquation(0);	// BLEND_FUNC_ADD
		break;
	case s2::FBM_ADD:
		ctx->SetBlend(2, 2);		// BLEND_GL_ONE, BLEND_GL_ONE
		ctx->SetBlendEquation(0);	// BLEND_FUNC_ADD
		break;
	case s2::FBM_SUBTRACT:
		ctx->SetBlend(2, 6);		// BLEND_GL_ONE, BLEND_GL_ONE_MINUS_SRC_ALPHA
		ctx->SetBlendEquation(1);	// BLEND_FUNC_SUBTRACT
		break;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (rs.blend != s2::BM_NULL) {
		const Camera* cam = CameraMgr::Instance()->GetCamera();
		if (cam->Type() == "ortho") {
			s2::DrawBlend::Draw(spr, params.mt);
		}
	} else if (rs.filter && rs.filter->GetMode() != s2::FM_NULL) {
		s2::RenderParams t = params;
		t.shader.filter = rs.filter;
		t.camera = rc;
		if (rs.filter->GetMode() == s2::FM_GAUSSIAN_BLUR) {
			int itrs = static_cast<s2::RFGaussianBlur*>(rs.filter)->GetIterations();
			SpriteGaussianBlur::Draw(spr, t, itrs);
		} else if (rs.filter->GetMode() == s2::FM_OUTER_GLOW) {
			int itrs = static_cast<s2::RFOuterGlow*>(rs.filter)->GetIterations();
			SpriteOuterGlow::Draw(spr, t, itrs);
		} else {
			if (params.set_shader) {
				mgr->SetShader(sl::FILTER);
			}
			sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
			shader->SetMode(sl::FILTER_MODE(rs.filter->GetMode()));
			switch (rs.filter->GetMode())
			{
			case s2::FM_EDGE_DETECTION:
				{
					s2::RFEdgeDetection* ed = static_cast<s2::RFEdgeDetection*>(rs.filter);
					sl::EdgeDetectProg* prog = static_cast<sl::EdgeDetectProg*>(shader->GetProgram(sl::FM_EDGE_DETECTION));
					prog->SetBlend(ed->GetBlend());
				}
				break;
			}
			spr->GetSymbol()->Draw(t, spr);
			DrawAnchor(spr, t);
		}
	} else {
		if (params.set_shader) {
			mgr->SetShader(sl::SPRITE2);
		}
		s2::RenderParams t = params;
		t.camera = rc;
		spr->GetSymbol()->Draw(t, spr);
		DrawAnchor(spr, t);
	}
}

void SpriteRenderer::InvalidRect(const Sprite* spr, const sm::mat4& mt)
{
	if (!spr->IsVisible()) {
		return;
	}

//	spr->GetSymbol()->InvalidRect(spr->GetTransMatrix() * mt);
}

void SpriteRenderer::DrawAnchor(const Sprite* spr, const s2::RenderParams& params)
{
	if (!spr->IsAnchor() || !Config::Instance()->GetSettings().draw_anchor) {
		return;
	}

	std::vector<sm::vec2> bound;
	sm::rect rect = spr->GetBounding()->GetSize();
	bound.push_back(sm::vec2(rect.xmin, rect.ymin));
	bound.push_back(sm::vec2(rect.xmax, rect.ymin));
	bound.push_back(sm::vec2(rect.xmax, rect.ymax));
	bound.push_back(sm::vec2(rect.xmin, rect.ymax));		
	for (int i = 0, n = bound.size(); i < n; ++i) {
		bound[i] = params.mt * bound[i];
	}
	s2::RVG::SetColor(BLACK);
	s2::RVG::LineWidth(4);
	s2::RVG::Polyline(bound, true);
	s2::RVG::Line(bound[0], bound[2]);
	s2::RVG::Line(bound[1], bound[3]);
}

}