#include "EditPolygonCMPT.h"

namespace d2d
{

EditPolygonCMPT::EditPolygonCMPT(wxWindow* parent, const wxString& name,
								 EditPanel* editPanel, MultiShapesImpl* shapesImpl,
								 PropertySettingPanel* propertyPanel)
	: NodeCaptureCMPT<EditPolylineOP<DrawPolygonEdgeOP, SelectShapesOP> >(parent, name, editPanel, shapesImpl, propertyPanel)
{
}
} // d2d