
#ifndef EMODELING_BODY_EDIT_STAGE_H
#define EMODELING_BODY_EDIT_STAGE_H


#include <easyshape.h>

namespace emodeling
{

class BodyEditStage : public eshape::StagePanel
{
public:
	BodyEditStage(wxWindow* parent, wxTopLevelWindow* frame, 
		eshape::Symbol* symbol);

	//
	// eshape::StagePanel interface
	//
	virtual void loadShapes();

}; // BodyEditStage

}

#endif // EMODELING_BODY_EDIT_STAGE_H