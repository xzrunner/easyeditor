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

#include <shaderlab.h>

namespace ee
{

void SpriteRenderer::Draw(const Sprite* spr, 
						  const s2::RenderParams& params)
{
	if (!spr->visiable) {
		return;
	}

	s2::BlendMode blend;
	if (params.shader.blend != s2::BM_NULL) {
		blend = params.shader.blend;
	} else {
		blend = spr->GetShader().blend;
	}

	s2::FilterMode filter;
	if (params.shader.filter != s2::FM_NULL) {
		filter = params.shader.filter;
	} else {
		filter = spr->GetShader().filter;
	}

	s2::RenderCamera ct;
	if (params.camera.mode != s2::CM_ORTHO) {
		ct.mode = params.camera.mode;
	} else {
		ct.mode = spr->GetCamera().mode;
	}
	if (ct.mode == s2::CM_PERSPECTIVE_AUTO_HEIGHT) {
		if (params.camera.base_y == FLT_MAX) {
			ct.base_y = params.camera.base_y;
			std::vector<sm::vec2> bound;
			spr->GetBounding()->GetBoundPos(bound);
			for (int i = 0, n = bound.size(); i < n; ++i) {
				if (bound[i].y < ct.base_y) {
					ct.base_y = bound[i].y;
				}
			}
		} else {
			ct.base_y = params.camera.base_y;
		}
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (blend != s2::BM_NULL) {
		const Camera* cam = CameraMgr::Instance()->GetCamera();
		if (cam->Type() == "ortho") {
			SpriteBlend::Draw(spr, params.mt);
		}
	} else if (filter != s2::FM_NULL) {
		s2::RenderParams t = params;
		t.shader.filter = filter;
		t.camera = ct;
		if (t.shader.filter == s2::FM_GAUSSIAN_BLUR) {
			SpriteGaussianBlur::Draw(spr, t);
		} else if (t.shader.filter == s2::FM_OUTER_GLOW) {
			SpriteOuterGlow::Draw(spr, t);
		} else {
			if (params.set_shader) {
				mgr->SetShader(sl::FILTER);
			}
			sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
			shader->SetMode(sl::FILTER_MODE(filter));
			DrawImpl(spr, t);
		}
	} else {
		if (params.set_shader) {
			mgr->SetShader(sl::SPRITE2);
		}
		s2::RenderParams t = params;
		t.camera = ct;
		DrawImpl(spr, t);
	}
}

void SpriteRenderer::InvalidRect(const Sprite* sprite, const sm::mat4& mt)
{
	if (!sprite->visiable) {
		return;
	}

	sm::mat4 t;
	sprite->GetTransMatrix(t);
	t = t * mt;
	
	sprite->GetSymbol().InvalidRect(t);
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

	symbol->Draw(t);
}

void SpriteRenderer::DrawImpl(const Sprite* spr, 
							  const s2::RenderParams& params)
{
	sm::mat4 t;
	spr->GetTransMatrix(t);
	t = t * params.mt;

	s2::RenderColor col_new;

	col_new.mul	 = spr->GetColor().mul * params.color.mul;
	col_new.add	 = spr->GetColor().add + params.color.add;
	col_new.rmap = spr->GetColor().rmap.MapMul(params.color.rmap, params.color.gmap, params.color.bmap);
	col_new.gmap = spr->GetColor().gmap.MapMul(params.color.rmap, params.color.gmap, params.color.bmap);
	col_new.bmap = spr->GetColor().bmap.MapMul(params.color.rmap, params.color.gmap, params.color.bmap);

	s2::RenderParams _trans = params;
	_trans.mt = t;
	_trans.color = col_new;

	spr->GetSymbol().Draw(_trans, spr);

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