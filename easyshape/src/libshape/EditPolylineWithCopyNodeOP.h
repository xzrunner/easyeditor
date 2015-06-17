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
		d2d::ViewPanelMgr* view_panel_mgr)
		: EditPolylineOP<T, SelectNodesOP>(editPanel, shapesImpl, view_panel_mgr, NULL, NULL)
	{}

	virtual bool OnKeyDown(int keyCode)
	{
		if (EditPolylineOP<T, SelectNodesOP>::OnKeyDown(keyCode)) 
			return true;

		// todo EditPolylineOP 
// 		if (wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_CONTROL_V))
// 			m_selectOP->fetchSelectedNode(m_polyline);

		return false;
	}

}; // EditPolylineWithCopyNodeOP

}

#include "EditPolylineWithCopyNodeOP.h"

#endif // _LIBSHAPE_EDIT_POLYLINE_WITH_COPY_NODE_OP_H_