#include "DrawLineCMPT.h"
#include "DrawPenLineOP.h"
#include "EditPolylineOP.h"
#include "DrawPencilLineCMPT.h"

namespace libshape
{

DrawLineCMPT::DrawLineCMPT(wxWindow* parent, const wxString& name, wxWindow* stage_wnd,d2d::EditPanelImpl* stage, 
						   d2d::MultiShapesImpl* shapesImpl, d2d::PropertySettingPanel* propertyPanel,
						   d2d::ViewPanelMgr* view_panel_mgr)
	: d2d::AbstractEditCMPT(parent, name, stage, wxT("Tools"))
{
 	// draw polyline with pen, node capture
	{
		d2d::OneFloatValueCMPT* cmpt = new d2d::OneFloatValueCMPT(this, "pen", stage, "node capture", 5, 30, 10);
		d2d::AbstractEditOP* op = new EditPolylineOP<DrawPenLineOP, d2d::SelectShapesOP>(stage_wnd, stage, shapesImpl, propertyPanel, view_panel_mgr, cmpt, cmpt);
		cmpt->SetEditOP(op);
		addChild(cmpt);
	}
 	// draw polyline with pencil, simplify threshold
	{
		addChild(new DrawPencilLineCMPT(this, wxT("pencil"), stage_wnd, stage, shapesImpl));
	}
}

wxSizer* DrawLineCMPT::initLayout()
{
	return initChildrenLayout();
}

} // d2d