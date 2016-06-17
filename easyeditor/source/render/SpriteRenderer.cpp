#include "SpriteRenderer.h"
#include "SpriteBlend.h"
#include "Sprite.h"
#include "Symbol.h"
#include "Math2D.h"
#include "BoundingBox.h"
#include "EE_RVG.h"
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

#include <shaderlab.h>

namespace ee
{

void SpriteRenderer::Draw(const Sprite* spr, 
						  const s2::RenderParams& params)
{
	if (!spr->IsVisible()) {
		return;
	}

	s2::RenderShader rs = spr->GetShader() * params.shader;
	s2::RenderCamera rc = spr->GetCamera() * params.camera;

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (rs.blend != s2::BM_NULL) {
		const Camera* cam = CameraMgr::Instance()->GetCamera();
		if (cam->Type() == "ortho") {
			SpriteBlend::Draw(spr, params.mt);
		}
	} else if (rs.filter->GetMode() != s2::FM_NULL) {
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
			DrawImpl(spr, t);
		}
	} else {
		if (params.set_shader) {
			mgr->SetShader(sl::SPRITE2);
		}
		s2::RenderParams t = params;
		t.camera = rc;
		DrawImpl(spr, t);
	}
}

void SpriteRenderer::InvalidRect(const Sprite* sprite, const sm::mat4& mt)
{
	if (!sprite->IsVisible()) {
		return;
	}

	sprite->GetSymbol().InvalidRect(sprite->GetTransMatrix() * mt);
}

void SpriteRenderer::Draw(const Symbol* symbol, 
						  const s2::RenderParams& params /*= s2::RenderParams()*/,
						  const sm::vec2& pos, 
						  float angle/* = 0.0f*/, 
						  float xScale/* = 1.0f*/, 
						  float yScale/* = 1.0f*/, 
						  float xShear/* = 0.0f*/, 
						  float yShear/* = 0.0f*/)
{
	sm::mat4 mt;
	mt.SetTransformation(pos.x, pos.y, angle, xScale, yScale, 0, 0, xShear, yShear);
	mt = mt * params.mt;

	s2::RenderParams t = params;
	t.mt = mt;

	if (t.shader.blend != s2::BM_NULL) {
		;
	} else if (t.shader.filter && t.shader.filter->GetMode() != s2::FM_NULL) {
		;
	} else {
		if (t.set_shader) {
			sl::ShaderMgr::Instance()->SetShader(sl::SPRITE2);
		}
	}

	symbol->Draw(t);
}

void SpriteRenderer::DrawImpl(const Sprite* spr, 
							  const s2::RenderParams& params)
{
	spr->GetSymbol().Draw(params, spr);

	if (spr->IsAnchor() && Config::Instance()->GetSettings().draw_anchor) 
	{
		std::vector<sm::vec2> bound;
		spr->GetBounding()->GetBoundPos(bound);
		for (int i = 0, n = bound.size(); i < n; ++i) {
			bound[i] = Math2D::TransVector(bound[i], params.mt);
		}
		RVG::Color(BLACK);
		RVG::LineWidth(4);
		RVG::Polyline(bound, true);
		RVG::Line(bound[0], bound[2]);
		RVG::Line(bound[1], bound[3]);
	}
}

}