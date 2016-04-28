#ifndef _EASYMESH_EDIT_NW_CMPT_H_
#define _EASYMESH_EDIT_NW_CMPT_H_

#include <ee/EditCMPT.h>

namespace emesh
{

class StagePanel;
class EditNWCMPT : public ee::EditCMPT
{
public:
	EditNWCMPT(wxWindow* parent, const std::string& name,
		StagePanel* stage);

protected:
	virtual wxSizer* InitLayout();

private:
	void OnReset(wxCommandEvent& event);

private:
	StagePanel* m_stage;

}; // EditNWCMPT

}

#endif // _EASYMESH_EDIT_NW_CMPT_H_