#ifndef _EASYTERRAIN2D_CREATE_REGION_CMPT_H_
#define _EASYTERRAIN2D_CREATE_REGION_CMPT_H_

#include "CreateRegionOP.h"

#include <easyshape.h>

namespace eterrain2d
{

class StagePanel;

class CreateRegionCMPT : public libshape::NodeCaptureCMPT<libshape::EditPolylineOP<CreateRegionOP, d2d::SelectShapesOP> >
{
public:
	CreateRegionCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage);

protected:
	virtual wxSizer* initLayout();

private:
	void OnChangeDisplayTriangles(wxCommandEvent& event);

}; // CreateRegionCMPT

}

#endif // _EASYTERRAIN2D_CREATE_REGION_CMPT_H_