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
						  const d2d::Matrix& mt,
						  const Colorf& mul, 
						  const Colorf& add,
						  const Colorf& r_trans,
						  const Colorf& g_trans,
						  const Colorf& b_trans,
						  bool multi_draw) const
{
	if (!sprite->visiable) {
		return;
	}

	if (!multi_draw || sprite->GetBlendMode() == BM_NORMAL) {
		DrawImpl(sprite, mt, mul, add, r_trans, g_trans, b_trans);
	} else {
		SpriteBlend::Instance()->Draw(sprite, mt);
	}
}

void SpriteRenderer::DrawWithoutBlend(const ISprite* sprite, const d2d::Matrix& mt) const
{
	if (!sprite->visiable) {
		return;
	}

	DrawImpl(sprite, mt);
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
						  const Colorf& mul /*= Colorf(1,1,1,1)*/,
						  const Colorf& add /*= Colorf(0,0,0,0)*/,
						  const Colorf& r_trans,
						  const Colorf& g_trans,
						  const Colorf& b_trans) const
{
	Matrix t;
	t.setTransformation(pos.x, pos.y, angle, xScale, yScale, 0, 0, xShear, yShear);
	t = mt * t;
	symbol->Draw(t, mul, add, r_trans, g_trans, b_trans);
}

void SpriteRenderer::DrawImpl(const ISprite* sprite, 
							  const d2d::Matrix& mt,
							  const Colorf& mul, 
							  const Colorf& add,
							  const Colorf& r_trans,
							  const Colorf& g_trans,
							  const Colorf& b_trans) const
{
	Matrix t;
	sprite->GetTransMatrix(t);
	t = mt * t;

	Colorf _mul = cMul(sprite->multiCol, mul),
		_add = cAdd(sprite->addCol, add);

	Colorf _r_trans, _g_trans, _b_trans;

	_r_trans.r = sprite->r_trans.r * r_trans.r + sprite->r_trans.g * g_trans.r + sprite->r_trans.b * b_trans.r;
	_r_trans.g = sprite->r_trans.r * r_trans.g + sprite->r_trans.g * g_trans.g + sprite->r_trans.b * b_trans.g;
	_r_trans.b = sprite->r_trans.r * r_trans.b + sprite->r_trans.g * g_trans.b + sprite->r_trans.b * b_trans.b;

	_g_trans.r = sprite->g_trans.r * r_trans.r + sprite->g_trans.g * g_trans.r + sprite->g_trans.b * b_trans.r;
	_g_trans.g = sprite->g_trans.r * r_trans.g + sprite->g_trans.g * g_trans.g + sprite->g_trans.b * b_trans.g;
	_g_trans.b = sprite->g_trans.r * r_trans.b + sprite->g_trans.g * g_trans.b + sprite->g_trans.b * b_trans.b;

	_b_trans.r = sprite->b_trans.r * r_trans.r + sprite->b_trans.g * g_trans.r + sprite->b_trans.b * b_trans.r;
	_b_trans.g = sprite->b_trans.r * r_trans.g + sprite->b_trans.g * g_trans.g + sprite->b_trans.b * b_trans.g;
	_b_trans.b = sprite->b_trans.r * r_trans.b + sprite->b_trans.g * g_trans.b + sprite->b_trans.b * b_trans.b;

	sprite->GetSymbol().Draw(t, _mul, _add, _r_trans, _g_trans, _b_trans, sprite);

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