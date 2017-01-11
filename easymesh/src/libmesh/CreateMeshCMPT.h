#ifndef _EASYMESH_CREATE_MESH_CMPT_H_
#define _EASYMESH_CREATE_MESH_CMPT_H_

#include <ee/EditCMPT.h>

#include <sprite2/MeshType.h>

namespace emesh
{

class StagePanel;
class CreateMeshCMPT : public ee::EditCMPT
{
public:
	CreateMeshCMPT(wxWindow* parent, const std::string& name,
		StagePanel* stage);
	virtual ~CreateMeshCMPT();

	void SetEditOP(s2::MeshType type);

protected:
	virtual wxSizer* InitLayout();

private:
	void OnCopy(wxCommandEvent& event);
	void OnClear(wxCommandEvent& event);

private:
	StagePanel* m_stage;

	ee::EditOP *m_network_op, *m_strip_op, *m_skeleton_op;

}; // CreateMeshCMPT

}

#endif // _EASYMESH_CREATE_MESH_CMPT_H_