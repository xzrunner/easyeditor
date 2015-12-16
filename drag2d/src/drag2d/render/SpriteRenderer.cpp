#include "SpriteRenderer.h"
#include "SpriteBlend.h"

#include "common/color_config.h"
#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "dataset/AbstractBV.h"
#include "render/PrimitiveDraw.h"

namespace d2d
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

void SpriteRenderer::Draw(const ISprite* sprite, 
						  const ISprite* root,
						  const d2d::Matrix& mt,
						  const ColorTrans& color,
						  bool multi_draw) const
{
	if (!sprite->visiable) {
		return;
	}

	if (!multi_draw || sprite->GetBlendMode() == BM_NORMAL) {
		DrawImpl(sprite, root, mt, color);
	} else {
		SpriteBlend::Instance()->Draw(sprite, mt);
	}
}

void SpriteRenderer::DrawWithoutBlend(const ISprite* sprite, const ISprite* root, const d2d::Matrix& mt) const
{
	if (!sprite->visiable) {
		return;
	}

	DrawImpl(sprite, root, mt);
}

void SpriteRenderer::InvalidRect(const ISprite* sprite, const Matrix& mt)
{
	if (!sprite->visiable) {
		return;
	}

	Matrix t;
	sprite->GetTransMatrix(t);
	t = mt * t;
	
	sprite->GetSymbol().InvalidRect(t);
}

void SpriteRenderer::Draw(const ISymbol* symbol, 
						  const d2d::Matrix& mt,
						  const Vector& pos, 
						  float angle/* = 0.0f*/, 
						  float xScale/* = 1.0f*/, 
						  float yScale/* = 1.0f*/, 
						  float xShear/* = 0.0f*/, 
						  float yShear/* = 0.0f*/, 
						  const ColorTrans& color) const
{
	Matrix t;
	t.setTransformation(pos.x, pos.y, angle, xScale, yScale, 0, 0, xShear, yShear);
	t = mt * t;
	symbol->Draw(t, color);
}

void SpriteRenderer::DrawImpl(const ISprite* sprite, 
							  const ISprite* root,
							  const d2d::Matrix& mt,
							  const ColorTrans& color) const
{
	Matrix t;
	sprite->GetTransMatrix(t);
	t = mt * t;

	ColorTrans col_new;

	col_new.multi = cMul(sprite->color.multi, color.multi);
	col_new.add = cAdd(sprite->color.add, color.add);

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
		sprite->GetBounding()->getBoundPos(bound);
		for (int i = 0, n = bound.size(); i < n; ++i) {
			bound[i] = d2d::Math::transVector(bound[i], mt);
		}
		PrimitiveDraw::drawPolyline(bound, BLACK, true, 4);
		PrimitiveDraw::drawLine(bound[0], bound[2], BLACK, 4);
		PrimitiveDraw::drawLine(bound[1], bound[3], BLACK, 4);
	}
}

} // d2d