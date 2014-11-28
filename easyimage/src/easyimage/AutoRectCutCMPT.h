#ifndef _EASYIMAGE_AUTO_RECT_CUT_CMPT_H_
#define _EASYIMAGE_AUTO_RECT_CUT_CMPT_H_

#include <drag2d.h>

namespace eimage
{

class AutoRectCutCMPT : public d2d::AbstractEditCMPT
{
public:
	AutoRectCutCMPT(wxWindow* parent, const wxString& name,
		d2d::EditPanel* editpanel);

protected:
	virtual wxSizer* initLayout();

	void OnAddRect(wxCommandEvent& event);

private:
	wxChoice *m_width_choice, *m_height_choice;

}; // AutoRectCutCMPT

}

#endif // _EASYIMAGE_AUTO_RECT_CUT_CMPT_H_