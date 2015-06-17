#include "DrawLineCMPT.h"
#include "DrawPenLineOP.h"
#include "EditPolylineOP.h"
#include "DrawPencilLineCMPT.h"

namespace libshape
{

DrawLineCMPT::DrawLineCMPT(wxWindow* parent, const wxString& name, d2d::EditPanel* editPanel, 
						   d2d::MultiShapesImpl* shapesImpl, d2d::PropertySettingPanel* propertyPanel,
						   d2d::ViewPanelMgr* view_panel_mgr)
	: d2d::AbstractEditCMPT(parent, name, editPanel, wxT("Tools"))
{
 	// draw polyline with pen, node capture
	{
		d2d::OneFloatValueCMPT* cmpt = new d2d::OneFloatValueCMPT(this, "pen", editPanel, "node capture", 5, 30, 10);
		d2d::AbstractEditOP* op = new EditPolylineOP<DrawPenLineOP, d2d::SelectShapesOP>(editPanel, shapesImpl, propertyPanel, view_panel_mgr, cmpt, cmpt);
		cmpt->SetEditOP(op);
		addChild(cmpt);
	}
 	// draw polyline with pencil, simplify threshold
	{
		addChild(new DrawPencilLineCMPT(this, wxT("pencil"), editPanel, shapesImpl));
	}
}

wxSizer* DrawLineCMPT::initLayout()
{
	return initChildrenLayout();
}

} // d2d