#include "CreateRegionOP.h"
#include "StagePanel.h"

namespace eterrain2d
{

CreateRegionOP::CreateRegionOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
							   ee::MultiShapesImpl* shapesImpl)
	: libshape::DrawPolygonOP(wnd, stage, shapesImpl)
	, m_shapesImpl(shapesImpl)
{
}

bool CreateRegionOP::OnDraw() const
{
	if (libshape::DrawPolygonOP::OnDraw()) {
		return true;
	}

	m_shapesImpl->TraverseShapes(ee::DrawShapesVisitor(ee::Rect()), ee::DT_VISIBLE);

	return false;
}

}