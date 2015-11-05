#include "CreateRegionOP.h"
#include "StagePanel.h"

namespace eterrain2d
{

CreateRegionOP::CreateRegionOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
							   d2d::MultiShapesImpl* shapesImpl)
	: libshape::DrawPolygonOP(wnd, stage, shapesImpl)
	, m_shapesImpl(shapesImpl)
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