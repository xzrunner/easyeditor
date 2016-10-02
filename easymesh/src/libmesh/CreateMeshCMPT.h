#ifndef _EASYMESH_CREATE_MESH_CMPT_H_
#define _EASYMESH_CREATE_MESH_CMPT_H_

#include <ee/EditCMPT.h>

namespace emesh
{

class StagePanel;
class CreateMeshCMPT : public ee::EditCMPT
{
public:
	CreateMeshCMPT(wxWindow* parent, const std::string& name,
		StagePanel* stage);
	virtual ~CreateMeshCMPT();

protected:
	virtual wxSizer* InitLayout();

private:
	void onChangeType(wxCommandEvent& event);
	void onCopy(wxCommandEvent& event);
	void onClear(wxCommandEvent& event);

private:
	StagePanel* m_stage;

	ee::EditOP *m_mesh_op, *m_strip_op, *m_skeleton_op;

}; // CreateMeshCMPT

}

#endif // _EASYMESH_CREATE_MESH_CMPT_H_