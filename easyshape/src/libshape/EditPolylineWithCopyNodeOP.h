#ifndef _LIBSHAPE_EDIT_POLYLINE_WITH_COPY_NODE_OP_H_
#define _LIBSHAPE_EDIT_POLYLINE_WITH_COPY_NODE_OP_H_

#include "EditPolylineOP.h"
#include "SelectNodesOP.h"

namespace libshape
{

template <typename T>
class EditPolylineWithCopyNodeOP : public EditPolylineOP<T, SelectNodesOP>
{
public:
	EditPolylineWithCopyNodeOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
		d2d::PropertySettingPanel* propertyPanel)
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

#endif // _LIBSHAPE_EDIT_POLYLINE_WITH_COPY_NODE_OP_H_