#ifndef _EASYTERRAIN2D_CREATE_REGION_CMPT_H_
#define _EASYTERRAIN2D_CREATE_REGION_CMPT_H_

#include <easyshape.h>

namespace eterrain2d
{

class StagePanel;

class CreateRegionCMPT : public ee::OneFloatValueCMPT
{
public:
	CreateRegionCMPT(wxWindow* parent, const std::string& name,
		StagePanel* stage);

// 	//
// 	// interface OneFloatValue
// 	//
// 	virtual float GetValue() const;

protected:
	virtual wxSizer* InitLayout();

private:
	void OnChangeDisplayTriangles(wxCommandEvent& event);

}; // CreateRegionCMPT

}

#endif // _EASYTERRAIN2D_CREATE_REGION_CMPT_H_