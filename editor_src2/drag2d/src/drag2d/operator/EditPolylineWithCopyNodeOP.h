#pragma once

#include "EditPolylineOP.h"
#include "SelectNodesOP.h"

namespace d2d
{
	template <typename T>
	class EditPolylineWithCopyNodeOP : public EditPolylineOP<T, SelectNodesOP>
	{
 	public:
 		EditPolylineWithCopyNodeOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl,
			PropertySettingPanel* propertyPanel)
 			: EditPolylineOP<T, SelectNodesOP>(editPanel, shapesImpl, propertyPanel, NULL)
 		{}
 
 		virtual bool onKeyDown(int keyCode)
 		{
 			if (EditPolylineOP<T, SelectNodesOP>::onKeyDown(keyCode)) 
				return true;
 
 			if (wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_CONTROL_V))
 				m_selectOP->fetchSelectedNode(m_polyline);
 
 			return false;
 		}

	}; // EditPolylineWithCopyNodeOP
}

#include "EditPolylineWithCopyNodeOP.h"

