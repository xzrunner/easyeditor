#include "CreateRegionOP.h"
#include "StagePanel.h"

namespace eterrain2d
{

CreateRegionOP::CreateRegionOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl)
	: libshape::DrawPolygonOP(editPanel, shapesImpl)
{
}

bool CreateRegionOP::onDraw() const
{
	if (libshape::DrawPolygonOP::onDraw()) {
		return true;
	}

	m_shapesImpl->traverseShapes(d2d::DrawShapesVisitor(), d2d::DT_VISIBLE);

	return false;
}

}