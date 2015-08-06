#ifndef _LIBSHAPE_EDIT_POLYLINE_OP_H_
#define _LIBSHAPE_EDIT_POLYLINE_OP_H_

#include <drag2d.h>

#include "IOperaterBase.h"

namespace libshape
{

class ChainShape;
class EditPolylineImpl;

template <typename TBase, typename TSelected>
class EditPolylineOP : public TBase, public IOperaterBase
{
public:
	EditPolylineOP(wxWindow* wnd, 
		d2d::EditPanelImpl* stage, 
		d2d::MultiShapesImpl* shapesImpl, 
		d2d::PropertySettingPanel* propertyPanel,
		d2d::ViewPanelMgr* view_panel_mgr,
		d2d::OneFloatValue* node_capture,
		d2d::AbstractEditCMPT* cmpt);
	virtual ~EditPolylineOP();

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool clear();

	//
	// interface IOperaterBase
	//
	virtual bool OnMouseLeftDownBase(int x, int y);
	virtual bool OnMouseRightDownBase(int x, int y);

protected:
	bool IsDirty() const;
	void ResetDirty();

private:
	EditPolylineImpl* m_impl;

	TSelected* m_select_op;

	d2d::OneFloatValue* m_node_capture;

}; // EditPolylineOP

}

#include "EditPolylineOP.cpp"

#endif // _LIBSHAPE_EDIT_POLYLINE_OP_H_