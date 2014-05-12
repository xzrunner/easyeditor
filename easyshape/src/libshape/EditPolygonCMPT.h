#ifndef _LIBSHAPE_EDIT_POLYGON_CMPT_H_
#define _LIBSHAPE_EDIT_POLYGON_CMPT_H_

#include <drag2d.h>

#include "EditPolylineOP.h"
#include "DrawPolygonEdgeOP.h"

namespace libshape
{

class EditPolygonCMPT : public NodeCaptureCMPT<EditPolylineOP<DrawPolygonEdgeOP, d2d::SelectShapesOP> >
{
public:
	EditPolygonCMPT(wxWindow* parent, const wxString& name,
		d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
		d2d::PropertySettingPanel* propertyPanel);

//		virtual void updateControlValue();

}; // EditPolygonCMPT

}

#endif // _LIBSHAPE_EDIT_POLYGON_CMPT_H_