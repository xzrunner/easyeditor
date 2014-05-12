#include "EditPolygonCMPT.h"
#include "DrawPolygonEdgeOP.h"

namespace libshape
{

EditPolygonCMPT::EditPolygonCMPT(wxWindow* parent, const wxString& name,
								 d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
								 d2d::PropertySettingPanel* propertyPanel)
	: NodeCaptureCMPT<EditPolylineOP<DrawPolygonEdgeOP, d2d::SelectShapesOP> >(parent, name, editPanel, shapesImpl, propertyPanel)
{
}

}