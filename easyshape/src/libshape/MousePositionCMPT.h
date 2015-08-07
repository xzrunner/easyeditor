#ifndef _LIBSHAPE_MOUSE_POSITION_CMPT_H_
#define _LIBSHAPE_MOUSE_POSITION_CMPT_H_

#include <drag2d.h>

namespace libshape
{

class MousePositionCMPT : public d2d::MousePositionCMPT
{
public:
	MousePositionCMPT(wxWindow* parent, const wxString& name, 
		wxWindow* stage_wnd, d2d::EditPanelImpl* stage);

}; // MousePositionCMPT

}

#endif // _LIBSHAPE_MOUSE_POSITION_CMPT_H_