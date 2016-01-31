#ifndef _EASYMESH_EDIT_MESH_CMPT_H_
#define _EASYMESH_EDIT_MESH_CMPT_H_



namespace emesh
{

class StagePanel;
class EditMeshCMPT : public ee::EditCMPT
{
public:
	EditMeshCMPT(wxWindow* parent, const wxString& name,
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