
#ifndef EMODELING_BODY_EDIT_STAGE_H
#define EMODELING_BODY_EDIT_STAGE_H

#include <drag2d.h>
#include <easyshape.h>

namespace emodeling
{
	class BodyEditStage : public libshape::StagePanel
	{
	public:
		BodyEditStage(wxWindow* parent, wxTopLevelWindow* frame, 
			libshape::Symbol* symbol);

		//
		// d2d::MultiShapesImpl interface
		//
		virtual bool InsertShape(d2d::IShape* shape);
		virtual bool RemoveShape(d2d::IShape* shape);

		//
		// libshape::StagePanel interface
		//
		virtual void loadShapes();

	}; // BodyEditStage
}

#endif // EMODELING_BODY_EDIT_STAGE_H