#include "DrawLineCMPT.h"
#include "NodeCaptureCMPT.h"
#include "DrawPenLineOP.h"
#include "EditPolylineOP.h"
#include "DrawPencilLineCMPT.h"

namespace libshape
{

DrawLineCMPT::DrawLineCMPT(wxWindow* parent, const wxString& name, d2d::EditPanel* editPanel, 
						   d2d::MultiShapesImpl* shapesImpl, d2d::PropertySettingPanel* propertyPanel)
	: d2d::AbstractEditCMPT(parent, name, editPanel, wxT("Tools"))
{
 	// draw polyline with pen, node capture
	{
		NodeCaptureCMPT* cmpt = new NodeCaptureCMPT(this, "pen", editPanel);
		d2d::AbstractEditOP* op = new EditPolylineOP<DrawPenLineOP, d2d::SelectShapesOP>(editPanel, shapesImpl, propertyPanel, cmpt, cmpt);
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