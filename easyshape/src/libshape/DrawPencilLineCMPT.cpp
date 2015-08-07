#include "DrawPencilLineCMPT.h"
#include "DrawPencilLineOP.h"

namespace libshape
{

DrawPencilLineCMPT::DrawPencilLineCMPT(wxWindow* parent, const wxString& name,
									   wxWindow* stage_wnd,d2d::EditPanelImpl* stage, 
									   d2d::MultiShapesImpl* shapesImpl)
	: d2d::OneFloatValueCMPT(parent, name, stage, "Simplify", 0, 100, 30, 0.1f)
{
	m_editOP = new DrawPencilLineOP(stage_wnd, stage, shapesImpl, this); 
}

}