#include "CreateRegionOP.h"
#include "StagePanel.h"

namespace eterrain2d
{

CreateRegionOP::CreateRegionOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl)
	: libshape::DrawPolygonOP(editPanel, shapesImpl)
{
}

bool CreateRegionOP::OnDraw() const
{
	if (libshape::DrawPolygonOP::OnDraw()) {
		return true;
	}

	m_shapesImpl->TraverseShapes(d2d::DrawShapesVisitor(d2d::Rect()), d2d::DT_VISIBLE);

	return false;
}

}