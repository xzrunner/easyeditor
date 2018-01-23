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

	void SetEditOP(int pm_mesh_type);

protected:
	virtual wxSizer* InitLayout() override;

private:
	void OnReset(wxCommandEvent& event);

private:
	StagePanel* m_stage;

	std::shared_ptr<ee::EditOP> 
		m_points_op = nullptr, 
		m_skin_op = nullptr;

}; // EditMeshCMPT

}

#endif // _EASYMESH_EDIT_MESH_CMPT_H_