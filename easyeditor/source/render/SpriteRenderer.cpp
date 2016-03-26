#include "SpriteRenderer.h"
#include "SpriteBlend.h"
#include "Sprite.h"
#include "Symbol.h"
#include "Math2D.h"
#include "Color.h"
#include "BoundingBox.h"
#include "EE_RVG.h"
#include "color_config.h"
#include "ShaderMgr.h"
#include "FilterShader.h"

namespace ee
{

SpriteRenderer* SpriteRenderer::m_instance = NULL;

SpriteRenderer* SpriteRenderer::Instance()
{
	if (!m_instance) {
		m_instance = new SpriteRenderer();
	}
	return m_instance;
}

SpriteRenderer::SpriteRenderer()
	: m_cam(NULL)
{
}

void SpriteRenderer::Draw(const Sprite* sprite, 
						  const Sprite* root,
						  const SpriteTrans& trans,
						  bool set_shader) const
{
	if (!sprite->visiable) {
		return;
	}

	BlendMode blend;
	if (trans.shader.blend != BM_NORMAL) {
		blend = trans.shader.blend;
	} else {
		blend = sprite->shader.blend;
	}

	FilterMode filter;
	if (trans.shader.filter != FM_NORMAL) {
		filter = trans.shader.filter;
	} else {
		filter = sprite->shader.filter;
	}

	ShaderMgr* mgr = ShaderMgr::Instance();
	if (blend != BM_NORMAL) {
		SpriteBlend::Instance()->Draw(sprite, trans.mt);
	} else if (filter != FM_NORMAL) {
		if (set_shader) {
			mgr->SetShader(ShaderMgr::FILTER);
		}
		FilterShader* shader = static_cast<FilterShader*>(mgr->GetShader(ShaderMgr::FILTER));
		shader->SetMode(FilterModes::Instance()->GetNameENFromID(filter));
		SpriteTrans t = trans;
		t.shader.filter = filter;
		DrawImpl(sprite, root, t);
	} else {
		if (set_shader) {
			mgr->SetShader(ShaderMgr::SPRITE);
		}
		SpriteTrans t = trans;
		t.shader.blend = blend;
		DrawImpl(sprite, root, trans);
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
						  const SpriteTrans& trans /*= SpriteTrans()*/,
						  const Vector& pos, 
						  float angle/* = 0.0f*/, 
						  float xScale/* = 1.0f*/, 
						  float yScale/* = 1.0f*/, 
						  float xShear/* = 0.0f*/, 
						  float yShear/* = 0.0f*/) const
{
	Matrix mt;
	mt.SetTransformation(pos.x, pos.y, angle, xScale, yScale, 0, 0, xShear, yShear);
	mt = trans.mt * mt;

	SpriteTrans t = trans;
	t.mt = mt;

	symbol->Draw(t);
}

void SpriteRenderer::DrawImpl(const Sprite* sprite, 
							  const Sprite* root,
							  const SpriteTrans& trans) const
{
	Matrix t;
	sprite->GetTransMatrix(t);
	t = trans.mt * t;

	ColorTrans col_new;

	col_new.multi = col_mul(sprite->color.multi, trans.color.multi);
	col_new.add = col_add(sprite->color.add, trans.color.add);

	col_new.r.r = sprite->color.r.r * trans.color.r.r + sprite->color.r.g * trans.color.g.r + sprite->color.r.b * trans.color.b.r;
	col_new.r.g = sprite->color.r.r * trans.color.r.g + sprite->color.r.g * trans.color.g.g + sprite->color.r.b * trans.color.b.g;
	col_new.r.b = sprite->color.r.r * trans.color.r.b + sprite->color.r.g * trans.color.g.b + sprite->color.r.b * trans.color.b.b;

	col_new.g.r = sprite->color.g.r * trans.color.r.r + sprite->color.g.g * trans.color.g.r + sprite->color.g.b * trans.color.b.r;
	col_new.g.g = sprite->color.g.r * trans.color.r.g + sprite->color.g.g * trans.color.g.g + sprite->color.g.b * trans.color.b.g;
	col_new.g.b = sprite->color.g.r * trans.color.r.b + sprite->color.g.g * trans.color.g.b + sprite->color.g.b * trans.color.b.b;

	col_new.b.r = sprite->color.b.r * trans.color.r.r + sprite->color.b.g * trans.color.g.r + sprite->color.b.b * trans.color.b.r;
	col_new.b.g = sprite->color.b.r * trans.color.r.g + sprite->color.b.g * trans.color.g.g + sprite->color.b.b * trans.color.b.g;
	col_new.b.b = sprite->color.b.r * trans.color.r.b + sprite->color.b.g * trans.color.g.b + sprite->color.b.b * trans.color.b.b;

	SpriteTrans _trans = trans;
	_trans.mt = t;
	_trans.color = col_new;

	sprite->GetSymbol().Draw(_trans, sprite, root);

	if (sprite->IsAnchor()) {
		std::vector<Vector> bound;
		sprite->GetBounding()->GetBoundPos(bound);
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