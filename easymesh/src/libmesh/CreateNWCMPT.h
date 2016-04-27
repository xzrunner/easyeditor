#ifndef _EASYMESH_CREATE_NW_CMPT_H_
#define _EASYMESH_CREATE_NW_CMPT_H_

#include <ee/EditCMPT.h>

namespace emesh
{

class StagePanel;
class CreateNWCMPT : public ee::EditCMPT
{
public:
	CreateNWCMPT(wxWindow* parent, const std::string& name,
		StagePanel* stage);
	virtual ~CreateNWCMPT();

protected:
	virtual wxSizer* InitLayout();

private:
	void onChangeType(wxCommandEvent& event);
	void onCopy(wxCommandEvent& event);
	void onClear(wxCommandEvent& event);

private:
	StagePanel* m_stage;

	ee::EditOP *m_mesh_op, *m_strip_op;

}; // CreateNWCMPT

}

#endif // _EASYMESH_CREATE_NW_CMPT_H_