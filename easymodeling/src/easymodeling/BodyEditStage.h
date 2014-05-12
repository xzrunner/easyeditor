
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
			d2d::ISymbol* symbol);
		BodyEditStage(wxWindow* parent, wxTopLevelWindow* frame,
			d2d::ISprite* sprite);

		//
		// d2d::MultiShapesImpl interface
		//
		virtual void removeShape(d2d::IShape* shape);
		virtual void insertShape(d2d::IShape* shape);

		//
		// libshape::StagePanel interface
		//
		virtual void loadShapes();

	}; // BodyEditStage
}

#endif // EMODELING_BODY_EDIT_STAGE_H