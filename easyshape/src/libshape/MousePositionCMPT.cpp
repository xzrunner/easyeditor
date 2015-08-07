#include "MousePositionCMPT.h"
#include "MousePositionOP.h"

namespace libshape
{

MousePositionCMPT::MousePositionCMPT(wxWindow* parent, const wxString& name, 
									 wxWindow* stage_wnd, d2d::EditPanelImpl* stage)
	: d2d::MousePositionCMPT(parent, name, stage_wnd, stage, false)
{
	m_editOP = new MousePositionOP(stage_wnd, stage, this);
}

}