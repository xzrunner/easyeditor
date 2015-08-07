#ifndef _DRAG2D_MOUSE_POSITION_CMPT_H_
#define _DRAG2D_MOUSE_POSITION_CMPT_H_

#include "AbstractEditCMPT.h"

namespace d2d
{

class Vector;

class MousePositionCMPT : public AbstractEditCMPT
{
public:
	MousePositionCMPT(wxWindow* parent, const wxString& name, 
		wxWindow* stage_wnd, EditPanelImpl* stage, bool create_op = true);

	void updatePosition(const Vector& pos);

protected:
	virtual wxSizer* initLayout();

private:
	wxTextCtrl *m_xText, *m_yText;

}; // MousePositionCMPT

}

#endif // _DRAG2D_MOUSE_POSITION_CMPT_H_