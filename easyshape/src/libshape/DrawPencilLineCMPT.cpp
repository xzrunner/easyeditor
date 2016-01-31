#include "DrawPencilLineCMPT.h"
#include "DrawPencilLineOP.h"

namespace libshape
{

DrawPencilLineCMPT::DrawPencilLineCMPT(wxWindow* parent, const wxString& name,
									   wxWindow* stage_wnd,ee::EditPanelImpl* stage)
	: ee::OneFloatValueCMPT(parent, name, stage, "Simplify", 0, 100, 30, 0.1f)
{
	m_editOP = new DrawPencilLineOP(stage_wnd, stage, this); 
}

}