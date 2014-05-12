#ifndef _EASYMESH_EDIT_UV_CMPT_H_
#define _EASYMESH_EDIT_UV_CMPT_H_

#include <drag2d.h>

namespace emesh
{

class StagePanel;

class EditUVCMPT : public d2d::AbstractEditCMPT
{
public:
	EditUVCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage);

protected:
	virtual wxSizer* initLayout();

private:
	void onMove(wxCommandEvent& event);
	void onReset(wxCommandEvent& event);

private:
	StagePanel* m_stage;

}; // EditUVCMPT

}

#endif // _EASYMESH_EDIT_UV_CMPT_H_