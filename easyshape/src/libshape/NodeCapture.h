#ifndef _EASYSHAPE_NODE_CAPTURE_H_
#define _EASYSHAPE_NODE_CAPTURE_H_



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
	ee::Vector pos;

	NodeAddr() : shape(NULL) {}

	void clear() {
		shape = NULL;
		pos.SetInvalid();
	}
};

class NodeCapture
{
public:
	NodeCapture(ee::MultiShapesImpl* shapesImpl, int tol);

	void captureEditable(const ee::Vector& pos, NodeAddr& result);
	void captureSelectable(const ee::Vector& pos, NodeAddr& result);

private:
	class RectQueryVisitor : public ee::Visitor
	{
	public:
		RectQueryVisitor(const ee::Vector& pos, float tolerance, NodeAddr& result);

		virtual void Visit(ee::Object* object, bool& next);

	private:
		bool Visit(PointShape* point);
		bool Visit(BezierShape* bezier);
		bool Visit(ChainShape* chain);
		bool Visit(CircleShape* circle);
		bool Visit(RectShape* rect);

	private:
		const ee::Vector& m_pos;
		float m_tolerance;

		ee::Rect m_rect;
		NodeAddr& m_result;

	}; // RectQueryVisitor

private:
	ee::MultiShapesImpl* m_shapesImpl;
	int m_tol;

}; // NodeCapture

}

#endif // _EASYSHAPE_NODE_CAPTURE_H_