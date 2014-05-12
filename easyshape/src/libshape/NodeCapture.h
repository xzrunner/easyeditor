#ifndef _LIBSHAPE_NODE_CAPTURE_H_
#define _LIBSHAPE_NODE_CAPTURE_H_

#include <drag2d.h>

namespace libshape
{

class BezierShape;
class ChainShape;
class CircleShape;
class RectShape;

struct NodeAddr
{
	d2d::IShape* shape;
	d2d::Vector pos;

	NodeAddr() : shape(NULL) {}

	void clear() {
		shape = NULL;
		pos.setInvalid();
	}
};

class NodeCapture
{
public:
	NodeCapture(d2d::MultiShapesImpl* shapesImpl, int tol);

	void captureEditable(const d2d::Vector& pos, NodeAddr& result);
	void captureSelectable(const d2d::Vector& pos, NodeAddr& result);

private:
	class RectQueryVisitor : public d2d::IVisitor
	{
	public:
		RectQueryVisitor(const d2d::Vector& pos, float tolerance, NodeAddr& result);

		virtual void visit(d2d::Object* object, bool& bFetchNext);

	private:
		bool visit(BezierShape* bezier);
		bool visit(ChainShape* chain);
		bool visit(CircleShape* circle);
		bool visit(RectShape* rect);

	private:
		const d2d::Vector& m_pos;
		float m_tolerance;

		d2d::Rect m_rect;
		NodeAddr& m_result;

	}; // RectQueryVisitor

private:
	d2d::MultiShapesImpl* m_shapesImpl;
	int m_tol;

}; // NodeCapture

}

#endif // _LIBSHAPE_NODE_CAPTURE_H_