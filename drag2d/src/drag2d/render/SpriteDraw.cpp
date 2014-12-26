#include "SpriteDraw.h"
#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"

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

	Colorf _r_trans = sprite->r_trans,
		_g_trans = sprite->g_trans,
		_b_trans = sprite->b_trans;

	sprite->getSymbol().draw(t, _mul, _add, _r_trans, _g_trans, _b_trans, sprite);
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