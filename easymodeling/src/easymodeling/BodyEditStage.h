
#ifndef EMODELING_BODY_EDIT_STAGE_H
#define EMODELING_BODY_EDIT_STAGE_H


#include <easyshape.h>

namespace emodeling
{

class BodyEditStage : public libshape::StagePanel
{
public:
	BodyEditStage(wxWindow* parent, wxTopLevelWindow* frame, 
		libshape::Symbol* symbol);

	//
	// libshape::StagePanel interface
	//
	virtual void loadShapes();

}; // BodyEditStage

}

#endif // EMODELING_BODY_EDIT_STAGE_H