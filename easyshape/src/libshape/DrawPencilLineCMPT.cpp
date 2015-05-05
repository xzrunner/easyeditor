#include "DrawPencilLineCMPT.h"
#include "DrawPencilLineOP.h"

namespace libshape
{

DrawPencilLineCMPT::DrawPencilLineCMPT(wxWindow* parent, const wxString& name,
									   d2d::EditPanel* editPanel, 
									   d2d::MultiShapesImpl* shapesImpl)
	: d2d::OneFloatValueCMPT(parent, name, editPanel, "Simplify", 0, 100, 30, 0.1f)
{
	m_editOP = new DrawPencilLineOP(editPanel, shapesImpl, this); 
}

}