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
	addChild(new NodeCaptureCMPT<EditPolylineOP<DrawPenLineOP, d2d::SelectShapesOP> >
		(this, wxT("pen"), editPanel, shapesImpl, propertyPanel));
	addChild(new DrawPencilLineCMPT(this, wxT("pencil"), editPanel, shapesImpl));
}

wxSizer* DrawLineCMPT::initLayout()
{
	return initChildrenLayout();
}

} // d2d