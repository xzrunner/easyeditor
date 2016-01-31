#include "MousePositionCMPT.h"
#include "MousePositionOP.h"

namespace libshape
{

MousePositionCMPT::MousePositionCMPT(wxWindow* parent, const wxString& name, 
									 wxWindow* stage_wnd, ee::EditPanelImpl* stage)
	: ee::MousePositionCMPT(parent, name, stage_wnd, stage, false)
{
	m_editOP = new MousePositionOP(stage_wnd, stage, this);
}

}