#include "DrawPolygon2CMPT.h"
#include "EditPolylineOP.h"
#include "DrawPolygonOP.h"
#include "DrawPencilPolygonCMPT.h"

namespace eshape
{

DrawPolygon2CMPT::DrawPolygon2CMPT(wxWindow* parent, const std::string& name, wxWindow* stage_wnd,
								   ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapesImpl, 
								   ee::PropertySettingPanel* property)
	: ee::EditCMPT(parent, name, stage, wxT("Tools"))
{
 	// draw polygon with pen, node capture
	{
		ee::OneFloatValueCMPT* cmpt = new ee::OneFloatValueCMPT(this, "pen", stage, "node capture", 5, 30, 10);
		ee::EditOP* op = new EditPolylineOP<DrawPolygonOP, ee::SelectShapesOP>
			(stage_wnd, stage, shapesImpl, property, cmpt, cmpt);
		cmpt->SetEditOP(op);
		addChild(cmpt);
	}
 	// draw polygon with pencil, simplify threshold
	{
		addChild(new DrawPencilPolygonCMPT(this, wxT("pencil"), stage_wnd, stage, shapesImpl));
	}
}

wxSizer* DrawPolygon2CMPT::InitLayout()
{
	return InitChildrenLayout();
}

}