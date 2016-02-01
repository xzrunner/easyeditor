#include "DrawPolygon2CMPT.h"
#include "EditPolylineOP.h"
#include "DrawPolygonOP.h"
#include "DrawPencilPolygonCMPT.h"

#include <ee/SelectShapesOP.h>

namespace eshape
{

DrawPolygon2CMPT::DrawPolygon2CMPT(wxWindow* parent, const std::string& name, wxWindow* stage_wnd,
								   ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl, 
								   ee::PropertySettingPanel* property)
	: ee::EditCMPT(parent, name, stage, "Tools")
{
 	// draw polygon with pen, node capture
	{
		ee::OneFloatValueCMPT* cmpt = new ee::OneFloatValueCMPT(this, "pen", stage, "node capture", 5, 30, 10);
		ee::EditOP* op = new EditPolylineOP<DrawPolygonOP, ee::SelectShapesOP>
			(stage_wnd, stage, shapes_impl, property, cmpt, cmpt);
		cmpt->SetEditOP(op);
		AddChild(cmpt);
	}
 	// draw polygon with pencil, simplify threshold
	{
		AddChild(new DrawPencilPolygonCMPT(this, "pencil", stage_wnd, stage, shapes_impl));
	}
}

wxSizer* DrawPolygon2CMPT::InitLayout()
{
	return InitChildrenLayout();
}

}