#ifndef _EASYSHAPE_NODE_CAPTURE_H_
#define _EASYSHAPE_NODE_CAPTURE_H_

#include <ee/Visitor.h>
#include <SM_Rect.h>

namespace ee { class Shape; class MultiShapesImpl; }

namespace eshape
{

class PointShape;
class BezierShape;
class ChainShape;
class CircleShape;
class RectShape;

struct NodeAddr
{
	ee::Shape* shape;
	sm::vec2 pos;
	bool pos_valid;

	NodeAddr() : shape(NULL) {}

	void clear() {
		shape = NULL;
		pos_valid = false;
	}
};

class NodeCapture
{
public:
	NodeCapture(ee::MultiShapesImpl* shapes_impl, int tol);

	void captureEditable(const sm::vec2& pos, NodeAddr& result);
	void captureSelectable(const sm::vec2& pos, NodeAddr& result);

private:
	class RectQueryVisitor : public ee::Visitor
	{
	public:
		RectQueryVisitor(const sm::vec2& pos, float tolerance, NodeAddr& result);

		virtual void Visit(ee::Object* object, bool& next);

	private:
		bool Visit(PointShape* point);
		bool Visit(BezierShape* bezier);
		bool Visit(ChainShape* chain);
		bool Visit(CircleShape* circle);
		bool Visit(RectShape* rect);

	private:
		const sm::vec2& m_pos;
		float m_tolerance;

		sm::rect m_rect;
		NodeAddr& m_result;

	}; // RectQueryVisitor

private:
	ee::MultiShapesImpl* m_shapes_impl;
	int m_tol;

}; // NodeCapture

}

#endif // _EASYSHAPE_NODE_CAPTURE_H_