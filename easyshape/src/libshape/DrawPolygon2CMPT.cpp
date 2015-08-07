#include "DrawPolygon2CMPT.h"
#include "EditPolylineOP.h"
#include "DrawPolygonOP.h"
#include "DrawPencilPolygonCMPT.h"

namespace libshape
{

DrawPolygon2CMPT::DrawPolygon2CMPT(wxWindow* parent, const wxString& name, wxWindow* stage_wnd,
								   d2d::EditPanelImpl* stage, d2d::MultiShapesImpl* shapesImpl, 
								   d2d::PropertySettingPanel* property, d2d::ViewPanelMgr* view_panel_mgr)
	: d2d::AbstractEditCMPT(parent, name, stage, wxT("Tools"))
{
 	// draw polygon with pen, node capture
	{
		d2d::OneFloatValueCMPT* cmpt = new d2d::OneFloatValueCMPT(this, "pen", stage, "node capture", 5, 30, 10);
		d2d::AbstractEditOP* op = new EditPolylineOP<DrawPolygonOP, d2d::SelectShapesOP>
			(stage_wnd, stage, shapesImpl, property, view_panel_mgr, cmpt, cmpt);
		cmpt->SetEditOP(op);
		addChild(cmpt);
	}
 	// draw polygon with pencil, simplify threshold
	{
		addChild(new DrawPencilPolygonCMPT(this, wxT("pencil"), stage_wnd, stage, shapesImpl));
	}
}

wxSizer* DrawPolygon2CMPT::initLayout()
{
	return initChildrenLayout();
}

} // d2d