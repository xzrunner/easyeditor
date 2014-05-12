#ifndef _LIBSHAPE_EDIT_NODES_CMPT_H_
#define _LIBSHAPE_EDIT_NODES_CMPT_H_

#include <drag2d.h>

namespace libshape
{

class MultiShapesImpl;

class EditNodesCMPT : public AbstractEditCMPT
{
public:
	EditNodesCMPT(wxWindow* parent, const wxString& name,
		d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl);

protected:
	virtual wxSizer* initLayout();

private:
	wxSizer* initEditPanel();

	void onSimplifyTrigger(wxCommandEvent& event);
	void onSmoothTrigger(wxCommandEvent& event);
	void onSaveEditChanged(wxCommandEvent& event);

private:
	wxSpinCtrl* m_simplifySpin;
	wxSpinCtrl* m_smoothSpin;

}; // EditNodesCMPT

}

#endif // _LIBSHAPE_EDIT_NODES_CMPT_H_