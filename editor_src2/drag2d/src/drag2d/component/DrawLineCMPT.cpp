#include "DrawLineCMPT.h"
#include "NodeCaptureCMPT.h"
#include "DrawPencilLineCMPT.h"

#include "operator/EditPolylineOP.h"
#include "operator/DrawPenLineOP.h"
#include "operator/SelectShapesOP.h"

namespace d2d
{

DrawLineCMPT::DrawLineCMPT(wxWindow* parent, const wxString& name, EditPanel* editPanel, 
						   MultiShapesImpl* shapesImpl, PropertySettingPanel* propertyPanel)
	: AbstractEditCMPT(parent, name, editPanel, wxT("Tools"))
{
	addChild(new NodeCaptureCMPT<EditPolylineOP<DrawPenLineOP, SelectShapesOP> >
		(this, wxT("pen"), editPanel, shapesImpl, propertyPanel));
	addChild(new DrawPencilLineCMPT(this, wxT("pencil"), editPanel, shapesImpl));
}

wxSizer* DrawLineCMPT::initLayout()
{
	return initChildrenLayout();
}

} // d2d