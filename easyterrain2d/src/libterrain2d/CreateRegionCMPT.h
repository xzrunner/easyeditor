#ifndef _EASYTERRAIN2D_CREATE_REGION_CMPT_H_
#define _EASYTERRAIN2D_CREATE_REGION_CMPT_H_

#include <easyshape.h>

namespace eterrain2d
{

class StagePanel;

class CreateRegionCMPT : public libshape::NodeCaptureCMPT
{
public:
	CreateRegionCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage);

	//
	// interface INodeCapture
	//
	virtual float GetScope() const;

protected:
	virtual wxSizer* initLayout();

private:
	void OnChangeDisplayTriangles(wxCommandEvent& event);

}; // CreateRegionCMPT

}

#endif // _EASYTERRAIN2D_CREATE_REGION_CMPT_H_