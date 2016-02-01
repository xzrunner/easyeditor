#ifndef _EASYSHAPE_EDIT_POLYLINE_OP_H_
#define _EASYSHAPE_EDIT_POLYLINE_OP_H_

#include "IOperaterBase.h"

namespace ee { class EditPanelImpl; class MultiShapesImpl; class PropertySettingPanel; class OneFloatValue; class EditCMPT; }

class wxWindow;

namespace eshape
{

class ChainShape;
class EditPolylineImpl;

template <typename TBase, typename TSelected>
class EditPolylineOP : public TBase, public IOperaterBase
{
public:
	EditPolylineOP(wxWindow* wnd, 
		ee::EditPanelImpl* stage, 
		ee::MultiShapesImpl* shapes_impl, 
		ee::PropertySettingPanel* property,
		ee::OneFloatValue* node_capture,
		ee::EditCMPT* cmpt);
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

	ee::OneFloatValue* m_node_capture;

}; // EditPolylineOP

}

#include "EditPolylineOP.cpp"

#endif // _EASYSHAPE_EDIT_POLYLINE_OP_H_