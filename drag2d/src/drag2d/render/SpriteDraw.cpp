#include "SpriteDraw.h"
#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "dataset/SpriteTools.h"

namespace d2d
{
void SpriteDraw::drawSprite(const ISprite* sprite, 
							const d2d::Matrix& mt,
							const Colorf& mul, 
							const Colorf& add,
							const Colorf& r_trans,
							const Colorf& g_trans,
							const Colorf& b_trans)
{
	if (!sprite->visiable)
		return;

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

	sprite->getSymbol().draw(t, _mul, _add, _r_trans, _g_trans, _b_trans, sprite);

	SpriteTools::DrawName(sprite, t);
}

void SpriteDraw::drawSprite(const ISymbol* symbol, 
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
							const Colorf& b_trans)
{
	Matrix t;
	t.setTransformation(pos.x, pos.y, angle, xScale, yScale, 0, 0, xShear, yShear);
	t = mt * t;
	symbol->draw(t, mul, add, r_trans, g_trans, b_trans);
}

} // d2d