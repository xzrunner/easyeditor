#include "DrawPencilPolygonCMPT.h"
#include "DrawPencilPolygonOP.h"

namespace libshape
{

DrawPencilPolygonCMPT::DrawPencilPolygonCMPT(wxWindow* parent, const wxString& name,
									   d2d::EditPanel* editPanel, 
									   d2d::MultiShapesImpl* shapesImpl)
	: d2d::OneFloatValueCMPT(parent, name, editPanel, "Simplify", 0, 100, 30, 0.1f)
{
	m_editOP = new DrawPencilPolygonOP(editPanel, shapesImpl, this); 
}

}