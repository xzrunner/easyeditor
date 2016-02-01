#include "CreateRegionOP.h"
#include "StagePanel.h"

namespace eterrain2d
{

CreateRegionOP::CreateRegionOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
							   ee::MultiShapesImpl* shapes_impl)
	: eshape::DrawPolygonOP(wnd, stage, shapes_impl)
	, m_shapes_impl(shapes_impl)
{
}

bool CreateRegionOP::OnDraw() const
{
	if (eshape::DrawPolygonOP::OnDraw()) {
		return true;
	}

	m_shapes_impl->TraverseShapes(ee::DrawShapesVisitor(ee::Rect()), ee::DT_VISIBLE);

	return false;
}

}