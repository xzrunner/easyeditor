#include "DrawLineCMPT.h"
#include "DrawPenLineOP.h"
#include "EditPolylineOP.h"
#include "DrawPencilLineCMPT.h"

namespace libshape
{

DrawLineCMPT::DrawLineCMPT(wxWindow* parent, const wxString& name, wxWindow* stage_wnd,ee::EditPanelImpl* stage, 
						   ee::MultiShapesImpl* shapesImpl, ee::PropertySettingPanel* propertyPanel)
	: ee::EditCMPT(parent, name, stage, wxT("Tools"))
{
 	// draw polyline with pen, node capture
	{
		ee::OneFloatValueCMPT* cmpt = new ee::OneFloatValueCMPT(this, "pen", stage, "node capture", 5, 30, 10);
		ee::EditOP* op = new EditPolylineOP<DrawPenLineOP, ee::SelectShapesOP>(stage_wnd, stage, shapesImpl, propertyPanel, cmpt, cmpt);
		cmpt->SetEditOP(op);
		addChild(cmpt);
	}
 	// draw polyline with pencil, simplify threshold
	{
		addChild(new DrawPencilLineCMPT(this, wxT("pencil"), stage_wnd, stage));
	}
}

wxSizer* DrawLineCMPT::InitLayout()
{
	return InitChildrenLayout();
}

}