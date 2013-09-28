#pragma once

#include "NodeCaptureCMPT.h"

#include "operator/EditPolylineOP.h"
#include "operator/DrawPolygonEdgeOP.h"
#include "operator/SelectShapesOP.h"

namespace d2d
{
	class EditPolygonCMPT : public NodeCaptureCMPT<EditPolylineOP<DrawPolygonEdgeOP, SelectShapesOP> >
	{
	public:
		EditPolygonCMPT(wxWindow* parent, const wxString& name,
			EditPanel* editPanel, MultiShapesImpl* shapesImpl,
			PropertySettingPanel* propertyPanel);

//		virtual void updateControlValue();

	}; // EditPolygonCMPT
}

