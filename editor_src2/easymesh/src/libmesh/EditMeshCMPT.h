#ifndef _EASYMESH_EDIT_MESH_CMPT_H_
#define _EASYMESH_EDIT_MESH_CMPT_H_

#include <drag2d.h>

namespace emesh
{

class StagePanel;
class EditMeshCMPT : public d2d::AbstractEditCMPT
{
public:
	EditMeshCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage);

protected:
	virtual wxSizer* initLayout();

}; // EditMeshCMPT

}

#endif // _EASYMESH_EDIT_MESH_CMPT_H_