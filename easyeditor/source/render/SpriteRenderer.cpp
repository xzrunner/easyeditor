#include "SpriteRenderer.h"
#include "SpriteBlend.h"
#include "Sprite.h"
#include "Symbol.h"
#include "Math2D.h"
#include "Color.h"
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

	BlendMode blend;
	if (trans.shader.blend != BM_NULL) {
		blend = trans.shader.blend;
	} else {
		blend = spr->rp->shader.blend;
	}

	FilterMode filter;
	if (trans.shader.filter != FM_NULL) {
		filter = trans.shader.filter;
	} else {
		filter = spr->rp->shader.filter;
	}

 	RenderCamera ct;
	if (trans.camera.mode != CM_ORTHO) {
		ct.mode = trans.camera.mode;
	} else {
		ct.mode = spr->rp->camera.mode;
	}
	if (ct.mode == CM_PERSPECTIVE_AUTO_HEIGHT) {
		if (trans.camera.base_y == FLT_MAX) {
			ct.base_y = trans.camera.base_y;
			std::vector<Vector> bound;
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
	if (blend != BM_NULL) {
		const Camera* cam = CameraMgr::Instance()->GetCamera();
		if (cam->Type() == "ortho") {
			SpriteBlend::Draw(spr, trans.mt);
		}
	} else if (filter != FM_NULL) {
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

void SpriteRenderer::InvalidRect(const Sprite* sprite, const Matrix& mt)
{
	if (!sprite->visiable) {
		return;
	}

	Matrix t;
	sprite->GetTransMatrix(t);
	t = mt * t;
	
	sprite->GetSymbol().InvalidRect(t);
}

void SpriteRenderer::Draw(const Symbol* symbol, 
						  const RenderParams& trans /*= RenderParams()*/,
						  const Vector& pos, 
						  float angle/* = 0.0f*/, 
						  float xScale/* = 1.0f*/, 
						  float yScale/* = 1.0f*/, 
						  float xShear/* = 0.0f*/, 
						  float yShear/* = 0.0f*/)
{
	Matrix mt;
	mt.SetTransformation(pos.x, pos.y, angle, xScale, yScale, 0, 0, xShear, yShear);
	mt = trans.mt * mt;

	RenderParams t = trans;
	t.mt = mt;

	symbol->Draw(t);
}

void SpriteRenderer::DrawImpl(const Sprite* spr, 
							  const Sprite* root,
							  const RenderParams& trans)
{
	Matrix t;
	spr->GetTransMatrix(t);
	t = trans.mt * t;

	RenderColor col_new;

	col_new.multi = col_mul(spr->rp->color.multi, trans.color.multi);
	col_new.add = col_add(spr->rp->color.add, trans.color.add);

	col_new.r.r = spr->rp->color.r.r * trans.color.r.r + spr->rp->color.r.g * trans.color.g.r + spr->rp->color.r.b * trans.color.b.r;
	col_new.r.g = spr->rp->color.r.r * trans.color.r.g + spr->rp->color.r.g * trans.color.g.g + spr->rp->color.r.b * trans.color.b.g;
	col_new.r.b = spr->rp->color.r.r * trans.color.r.b + spr->rp->color.r.g * trans.color.g.b + spr->rp->color.r.b * trans.color.b.b;

	col_new.g.r = spr->rp->color.g.r * trans.color.r.r + spr->rp->color.g.g * trans.color.g.r + spr->rp->color.g.b * trans.color.b.r;
	col_new.g.g = spr->rp->color.g.r * trans.color.r.g + spr->rp->color.g.g * trans.color.g.g + spr->rp->color.g.b * trans.color.b.g;
	col_new.g.b = spr->rp->color.g.r * trans.color.r.b + spr->rp->color.g.g * trans.color.g.b + spr->rp->color.g.b * trans.color.b.b;

	col_new.b.r = spr->rp->color.b.r * trans.color.r.r + spr->rp->color.b.g * trans.color.g.r + spr->rp->color.b.b * trans.color.b.r;
	col_new.b.g = spr->rp->color.b.r * trans.color.r.g + spr->rp->color.b.g * trans.color.g.g + spr->rp->color.b.b * trans.color.b.g;
	col_new.b.b = spr->rp->color.b.r * trans.color.r.b + spr->rp->color.b.g * trans.color.g.b + spr->rp->color.b.b * trans.color.b.b;

	RenderParams _trans = trans;
	_trans.mt = t;
	_trans.color = col_new;

	spr->GetSymbol().Draw(_trans, spr, root);

	if (spr->IsAnchor() && Config::Instance()->GetSettings().draw_anchor) 
	{
		std::vector<Vector> bound;
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