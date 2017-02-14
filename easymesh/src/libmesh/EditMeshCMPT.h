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
	virtual ~EditMeshCMPT();

	void SetEditOP(int pm_mesh_type);

protected:
	virtual wxSizer* InitLayout();

private:
	void OnReset(wxCommandEvent& event);

private:
	StagePanel* m_stage;

	ee::EditOP *m_points_op, *m_skin_op;

}; // EditMeshCMPT

}

#endif // _EASYMESH_EDIT_MESH_CMPT_H_