#include "MousePositionCMPT.h"
#include "MousePositionOP.h"

#include <ee/MousePositionOP.h>

namespace eshape
{

MousePositionCMPT::MousePositionCMPT(wxWindow* parent, const std::string& name, 
									 wxWindow* stage_wnd, ee::EditPanelImpl* stage)
	: ee::MousePositionCMPT(parent, name, stage_wnd, stage, false)
{
	m_editop = new MousePositionOP(stage_wnd, stage, this);
}

}