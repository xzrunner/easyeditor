#ifndef _EASYEDITOR_MOUSE_POSITION_CMPT_H_
#define _EASYEDITOR_MOUSE_POSITION_CMPT_H_

#include "EditCMPT.h"

namespace ee
{

class Vector;

class MousePositionCMPT : public EditCMPT
{
public:
	MousePositionCMPT(wxWindow* parent, const std::string& name, 
		wxWindow* stage_wnd, EditPanelImpl* stage, bool create_op = true);

	void updatePosition(const Vector& pos);

protected:
	virtual wxSizer* InitLayout();

private:
	wxTextCtrl *m_xText, *m_yText;

}; // MousePositionCMPT

}

#endif // _EASYEDITOR_MOUSE_POSITION_CMPT_H_