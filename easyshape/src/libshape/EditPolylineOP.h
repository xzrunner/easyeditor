#ifndef _LIBSHAPE_EDIT_POLYLINE_OP_H_
#define _LIBSHAPE_EDIT_POLYLINE_OP_H_

#include <drag2d.h>

#include "NodeCaptureCMPT.h"
#include "IOperaterBase.h"

namespace libshape
{

class ChainShape;
class EditPolylineImpl;

template <typename TBase, typename TSelected>
class EditPolylineOP : public TBase, public IOperaterBase
{
public:
	EditPolylineOP(d2d::EditPanel* editPanel, 
		d2d::MultiShapesImpl* shapesImpl, 
		d2d::PropertySettingPanel* propertyPanel,
		NodeCaptureCMPT<EditPolylineOP>* cmpt,
		float node_capture_scope = 0);
	virtual ~EditPolylineOP();

	virtual bool onKeyDown(int keyCode);
	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseMove(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onDraw(const d2d::Screen& scr) const;
	virtual bool clear();

	//
	// interface IOperaterBase
	//
	virtual bool OnMouseLeftDownBase(int x, int y);
	virtual bool OnMouseRightDownBase(int x, int y);

private:
	class NodeCapture : public INodeCapture
	{
	public:
		NodeCapture(float scope) : m_scope(scope) {}
		virtual float GetScope() const { return m_scope; }
	private:
		float m_scope;
	}; // NodeCapture

private:
	EditPolylineImpl* m_impl;

	TSelected* m_select_op;

	NodeCapture* m_node_capture;

}; // EditPolylineOP

}

#include "EditPolylineOP.cpp"

#endif // _LIBSHAPE_EDIT_POLYLINE_OP_H_