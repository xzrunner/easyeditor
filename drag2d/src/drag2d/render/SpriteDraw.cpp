#include "SpriteDraw.h"
#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"

namespace d2d
{
void SpriteDraw::drawSprite(const ISprite* sprite, const d2d::Matrix& mt,
							const Colorf& mul, const Colorf& add)
{
	if (!sprite->visiable)
		return;

	Matrix t;
	sprite->GetTransMatrix(t);
	t = mt * t;

	Colorf _mul = cMul(sprite->multiCol, mul),
		_add = cAdd(sprite->addCol, add);

	sprite->getSymbol().draw(t, _mul, _add, sprite);
}

void SpriteDraw::drawSprite(const ISymbol* symbol, const d2d::Matrix& mt,
	const Vector& pos, float angle/* = 0.0f*/, float xScale/* = 1.0f*/, 
	float yScale/* = 1.0f*/, float xShear/* = 0.0f*/, 
	float yShear/* = 0.0f*/, const Colorf& mul /*= Colorf(1,1,1,1)*/,
	const Colorf& add /*= Colorf(0,0,0,0)*/)
{
	Matrix t;
	t.setTransformation(pos.x, pos.y, angle, xScale, yScale, 0, 0, xShear, yShear);
	t = mt * t;
	symbol->draw(t, mul, add);
}

} // d2d