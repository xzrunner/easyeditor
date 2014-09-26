#ifndef _EASYMESH_CREATE_MESH_CMPT_H_
#define _EASYMESH_CREATE_MESH_CMPT_H_

#include <drag2d.h>

namespace emesh
{

class StagePanel;
class CreateMeshCMPT : public d2d::AbstractEditCMPT
{
public:
	CreateMeshCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage);
	virtual ~CreateMeshCMPT();

protected:
	virtual wxSizer* initLayout();

private:
	void onChangeType(wxCommandEvent& event);
	void onChangeUseRegion(wxCommandEvent& event);
	void onCopy(wxCommandEvent& event);
	void onClear(wxCommandEvent& event);

private:
	StagePanel* m_stage;

	d2d::AbstractEditOP *m_mesh_op, *m_strip_op;

}; // CreateMeshCMPT

}

#endif // _EASYMESH_CREATE_MESH_CMPT_H_