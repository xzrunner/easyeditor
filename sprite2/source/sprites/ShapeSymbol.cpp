#include "ShapeSymbol.h"
#include "Shape.h"
#include "RenderParams.h"

namespace s2
{

ShapeSymbol::ShapeSymbol(void* ud)
	: Symbol(ud)
	, m_shape(NULL)
{
}

void ShapeSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	if (m_shape) {
		m_shape->Draw(params.mt, params.color);
	}
}

void ShapeSymbol::SetShape(Shape* shape)
{
	rcobj_assign(m_shape, shape);
}

}