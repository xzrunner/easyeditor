#ifndef _LIBSHAPE_EDIT_POLYLINE_OP_H_
#define _LIBSHAPE_EDIT_POLYLINE_OP_H_

#include <drag2d.h>

#include "NodeCapture.h"
#include "NodeCaptureCMPT.h"

namespace libshape
{

class ChainShape;

template <typename TBase, typename TSelected>
class EditPolylineOP : public TBase
{
public:
	EditPolylineOP(d2d::EditPanel* editPanel, 
		d2d::MultiShapesImpl* shapesImpl, 
		d2d::PropertySettingPanel* propertyPanel,
		NodeCaptureCMPT<EditPolylineOP>* cmpt);
	virtual ~EditPolylineOP();

	virtual bool onKeyDown(int keyCode);
	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseMove(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onDraw() const;
	virtual bool clear();

private:
	void drawCaptured(const NodeAddr& captured) const;

	void checkActiveShape(const NodeAddr& captured);

private:
	class InterruptChainVisitor : public d2d::IVisitor
	{
	public:
		InterruptChainVisitor(const d2d::Vector& pos, int tol);

		virtual void visit(d2d::Object* object, bool& bFetchNext);

		ChainShape* getInterruptedChain() { return m_chain; }

	private:
		const d2d::Vector& m_pos;
		int m_tol;
		ChainShape* m_chain;

	}; // InterruptChainVisitor

	class NearestNodeVisitor : public d2d::IVisitor
	{
	public:
		NearestNodeVisitor(const d2d::Vector& pos, int tol);

		virtual void visit(d2d::Object* object, bool& bFetchNext);

		const d2d::Vector& getNearestNode() const {
			return m_nearest;
		}

	private:
		d2d::Vector m_pos;
		int m_tol;

		float m_dis;
		d2d::Vector m_nearest;

	}; // NearestNodeVisitor

private:
	static const int DRAG_SELECT_TOL = 5;

private:
	d2d::MultiShapesImpl* m_shapesImpl;

	d2d::PropertySettingPanel* m_propertyPanel;

	NodeCaptureCMPT<EditPolylineOP>* m_cmpt;
//		NodeAddr m_captured;

protected:
	NodeAddr m_capturedEditable, m_captureSelectable;

protected:
	TSelected* m_selectOP;
private:
	d2d::Vector m_lastLeftDownPos;
	bool m_bSelectOpen;

}; // EditPolylineOP

}

#include "EditPolylineOP.cpp"

#endif // _LIBSHAPE_EDIT_POLYLINE_OP_H_