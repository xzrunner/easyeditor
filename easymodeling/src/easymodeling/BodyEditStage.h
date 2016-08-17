#ifndef _EASYMODELING_BODY_EDIT_STAGE_H_
#define _EASYMODELING_BODY_EDIT_STAGE_H_

#include <easyshape.h>

namespace emodeling
{

class BodyEditStage : public eshape::StagePanel
{
public:
	BodyEditStage(wxWindow* parent, wxTopLevelWindow* frame, 
		eshape::Symbol* sym);

	//
	// eshape::StagePanel interface
	//
	virtual void loadShapes();

}; // BodyEditStage

}

#endif // _EASYMODELING_BODY_EDIT_STAGE_H_