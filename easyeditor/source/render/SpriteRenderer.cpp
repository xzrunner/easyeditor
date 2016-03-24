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
						  const Matrix& mt,
						  const ColorTrans& color,
						  bool multi_draw) const
{
	if (!sprite->visiable) {
		return;
	}

	ShaderMgr* mgr = ShaderMgr::Instance();
	if (multi_draw && sprite->shader.blend != BM_NORMAL) {
		SpriteBlend::Instance()->Draw(sprite, mt);
	} else if (sprite->shader.filter != FM_NORMAL) {
		mgr->SetShader(ShaderMgr::FILTER);
		FilterShader* shader = static_cast<FilterShader*>(mgr->GetShader(ShaderMgr::FILTER));
		shader->SetMode(FilterModes::Instance()->GetNameENFromID(sprite->shader.filter));
		DrawImpl(sprite, root, mt, color);
	} else {
// 		if (mgr->GetShader() == ShaderMgr::FILTER) {
// 			mgr->SetShader(ShaderMgr::SPRITE);
// 		}
		DrawImpl(sprite, root, mt, color);
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
						  const Matrix& mt,
						  const Vector& pos, 
						  float angle/* = 0.0f*/, 
						  float xScale/* = 1.0f*/, 
						  float yScale/* = 1.0f*/, 
						  float xShear/* = 0.0f*/, 
						  float yShear/* = 0.0f*/, 
						  const ColorTrans& color) const
{
	Matrix t;
	t.SetTransformation(pos.x, pos.y, angle, xScale, yScale, 0, 0, xShear, yShear);
	t = mt * t;
	symbol->Draw(t, color);
}

void SpriteRenderer::DrawImpl(const Sprite* sprite, 
							  const Sprite* root,
							  const Matrix& mt,
							  const ColorTrans& color) const
{
	Matrix t;
	sprite->GetTransMatrix(t);
	t = mt * t;

	ColorTrans col_new;

	col_new.multi = col_mul(sprite->color.multi, color.multi);
	col_new.add = col_add(sprite->color.add, color.add);

	col_new.r.r = sprite->color.r.r * color.r.r + sprite->color.r.g * color.g.r + sprite->color.r.b * color.b.r;
	col_new.r.g = sprite->color.r.r * color.r.g + sprite->color.r.g * color.g.g + sprite->color.r.b * color.b.g;
	col_new.r.b = sprite->color.r.r * color.r.b + sprite->color.r.g * color.g.b + sprite->color.r.b * color.b.b;

	col_new.g.r = sprite->color.g.r * color.r.r + sprite->color.g.g * color.g.r + sprite->color.g.b * color.b.r;
	col_new.g.g = sprite->color.g.r * color.r.g + sprite->color.g.g * color.g.g + sprite->color.g.b * color.b.g;
	col_new.g.b = sprite->color.g.r * color.r.b + sprite->color.g.g * color.g.b + sprite->color.g.b * color.b.b;

	col_new.b.r = sprite->color.b.r * color.r.r + sprite->color.b.g * color.g.r + sprite->color.b.b * color.b.r;
	col_new.b.g = sprite->color.b.r * color.r.g + sprite->color.b.g * color.g.g + sprite->color.b.b * color.b.g;
	col_new.b.b = sprite->color.b.r * color.r.b + sprite->color.b.g * color.g.b + sprite->color.b.b * color.b.b;

	sprite->GetSymbol().Draw(t, col_new, sprite, root);

	if (sprite->IsAnchor()) {
		std::vector<Vector> bound;
		sprite->GetBounding()->GetBoundPos(bound);
		for (int i = 0, n = bound.size(); i < n; ++i) {
			bound[i] = Math2D::TransVector(bound[i], mt);
		}
		RVG::Color(BLACK);
		RVG::LineWidth(4);
		RVG::Polyline(bound, true);
		RVG::Line(bound[0], bound[2]);
		RVG::Line(bound[1], bound[3]);
	}
}

}