#ifndef _LIBSHAPE_MOUSE_POSITION_CMPT_H_
#define _LIBSHAPE_MOUSE_POSITION_CMPT_H_



namespace libshape
{

class MousePositionCMPT : public ee::MousePositionCMPT
{
public:
	MousePositionCMPT(wxWindow* parent, const wxString& name, 
		wxWindow* stage_wnd, ee::EditPanelImpl* stage);

}; // MousePositionCMPT

}

#endif // _LIBSHAPE_MOUSE_POSITION_CMPT_H_