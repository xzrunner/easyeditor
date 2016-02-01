#ifndef _EASYSHAPE_MOUSE_POSITION_CMPT_H_
#define _EASYSHAPE_MOUSE_POSITION_CMPT_H_

#include <ee/MousePositionCMPT.h>

namespace eshape
{

class MousePositionCMPT : public ee::MousePositionCMPT
{
public:
	MousePositionCMPT(wxWindow* parent, const std::string& name, 
		wxWindow* stage_wnd, ee::EditPanelImpl* stage);

}; // MousePositionCMPT

}

#endif // _EASYSHAPE_MOUSE_POSITION_CMPT_H_