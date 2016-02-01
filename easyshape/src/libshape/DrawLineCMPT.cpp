#include "DrawLineCMPT.h"
#include "DrawPenLineOP.h"
#include "EditPolylineOP.h"
#include "DrawPencilLineCMPT.h"

#include <ee/SelectShapesOP.h>

namespace eshape
{

DrawLineCMPT::DrawLineCMPT(wxWindow* parent, const std::string& name, wxWindow* stage_wnd,ee::EditPanelImpl* stage, 
						   ee::MultiShapesImpl* shapes_impl, ee::PropertySettingPanel* property)
	: ee::EditCMPT(parent, name, stage, "Tools")
{
 	// draw polyline with pen, node capture
	{
		ee::OneFloatValueCMPT* cmpt = new ee::OneFloatValueCMPT(this, "pen", stage, "node capture", 5, 30, 10);
		ee::EditOP* op = new EditPolylineOP<DrawPenLineOP, ee::SelectShapesOP>(stage_wnd, stage, shapes_impl, property, cmpt, cmpt);
		cmpt->SetEditOP(op);
		AddChild(cmpt);
	}
 	// draw polyline with pencil, simplify threshold
	{
		AddChild(new DrawPencilLineCMPT(this, "pencil", stage_wnd, stage));
	}
}

wxSizer* DrawLineCMPT::InitLayout()
{
	return InitChildrenLayout();
}

}