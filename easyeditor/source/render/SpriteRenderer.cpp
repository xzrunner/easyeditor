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

#include <shaderlab.h>

namespace ee
{

void SpriteRenderer::Draw(const Sprite* spr, 
						  const Sprite* root,
						  const RenderParams& trans)
{
	if (!spr->visiable) {
		return;
	}

	s2::BlendMode blend;
	if (trans.shader.blend != s2::BM_NULL) {
		blend = trans.shader.blend;
	} else {
		blend = spr->GetShader().blend;
	}

	s2::FilterMode filter;
	if (trans.shader.filter != s2::FM_NULL) {
		filter = trans.shader.filter;
	} else {
		filter = spr->GetShader().filter;
	}

	s2::RenderCamera ct;
	if (trans.camera.mode != s2::CM_ORTHO) {
		ct.mode = trans.camera.mode;
	} else {
		ct.mode = spr->GetCamera().mode;
	}
	if (ct.mode == s2::CM_PERSPECTIVE_AUTO_HEIGHT) {
		if (trans.camera.base_y == FLT_MAX) {
			ct.base_y = trans.camera.base_y;
			std::vector<sm::vec2> bound;
			spr->GetBounding()->GetBoundPos(bound);
			for (int i = 0, n = bound.size(); i < n; ++i) {
				if (bound[i].y < ct.base_y) {
					ct.base_y = bound[i].y;
				}
			}
		} else {
			ct.base_y = trans.camera.base_y;
		}
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (blend != s2::BM_NULL) {
		const Camera* cam = CameraMgr::Instance()->GetCamera();
		if (cam->Type() == "ortho") {
			SpriteBlend::Draw(spr, trans.mt);
		}
	} else if (filter != s2::FM_NULL) {
		if (trans.set_shader) {
			mgr->SetShader(sl::FILTER);
		}
		sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
		shader->SetMode(sl::FILTER_MODE(filter));
		RenderParams t = trans;
		t.shader.filter = filter;
		t.camera = ct;
		DrawImpl(spr, root, t);
	} else {
		if (trans.set_shader) {
			mgr->SetShader(sl::SPRITE2);
		}
		RenderParams t = trans;
		t.camera = ct;
		DrawImpl(spr, root, t);
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
						  const RenderParams& trans /*= RenderParams()*/,
						  const sm::vec2& pos, 
						  float angle/* = 0.0f*/, 
						  float xScale/* = 1.0f*/, 
						  float yScale/* = 1.0f*/, 
						  float xShear/* = 0.0f*/, 
						  float yShear/* = 0.0f*/)
{
	sm::mat4 mt;
	mt.SetTransformation(pos.x, pos.y, angle, xScale, yScale, 0, 0, xShear, yShear);
	mt = mt * trans.mt;

	RenderParams t = trans;
	t.mt = mt;

	symbol->Draw(t);
}

void SpriteRenderer::DrawImpl(const Sprite* spr, 
							  const Sprite* root,
							  const RenderParams& trans)
{
	sm::mat4 t;
	spr->GetTransMatrix(t);
	t = t * trans.mt;

	s2::RenderColor col_new;

	col_new.mul	 = spr->GetColor().mul * trans.color.mul;
	col_new.add	 = spr->GetColor().add + trans.color.add;
	col_new.rmap = spr->GetColor().rmap.MapMul(trans.color.rmap, trans.color.gmap, trans.color.bmap);
	col_new.gmap = spr->GetColor().gmap.MapMul(trans.color.rmap, trans.color.gmap, trans.color.bmap);
	col_new.bmap = spr->GetColor().bmap.MapMul(trans.color.rmap, trans.color.gmap, trans.color.bmap);

	RenderParams _trans = trans;
	_trans.mt = t;
	_trans.color = col_new;

	spr->GetSymbol().Draw(_trans, spr, root);

	if (spr->IsAnchor() && Config::Instance()->GetSettings().draw_anchor) 
	{
		std::vector<sm::vec2> bound;
		spr->GetBounding()->GetBoundPos(bound);
		for (int i = 0, n = bound.size(); i < n; ++i) {
			bound[i] = Math2D::TransVector(bound[i], trans.mt);
		}
		RVG::Color(BLACK);
		RVG::LineWidth(4);
		RVG::Polyline(bound, true);
		RVG::Line(bound[0], bound[2]);
		RVG::Line(bound[1], bound[3]);
	}
}

}