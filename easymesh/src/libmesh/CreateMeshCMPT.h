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

	void SetEditOP(int pm_mesh_type);

protected:
	virtual wxSizer* InitLayout() override;

private:
	void OnCopy(wxCommandEvent& event);
	void OnClear(wxCommandEvent& event);

private:
	StagePanel* m_stage;

	std::shared_ptr<ee::EditOP> 
		m_network_op = nullptr, 
		m_strip_op = nullptr, 
		m_skeleton_op = nullptr;

}; // CreateMeshCMPT

}

#endif // _EASYMESH_CREATE_MESH_CMPT_H_