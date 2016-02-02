#ifndef _EASYMESH_EDIT_MESH_CMPT_H_
#define _EASYMESH_EDIT_MESH_CMPT_H_

#include <ee/EditCMPT.h>

namespace emesh
{

class StagePanel;
class EditMeshCMPT : public ee::EditCMPT
{
public:
	EditMeshCMPT(wxWindow* parent, const std::string& name,
		StagePanel* stage);

protected:
	virtual wxSizer* InitLayout();

private:
	void onReset(wxCommandEvent& event);

private:
	StagePanel* m_stage;

}; // EditMeshCMPT

}

#endif // _EASYMESH_EDIT_MESH_CMPT_H_