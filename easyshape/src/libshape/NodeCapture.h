#ifndef _EASYSHAPE_NODE_CAPTURE_H_
#define _EASYSHAPE_NODE_CAPTURE_H_

#include <ee/Visitor.h>
#include <ee/Shape.h>

#include <SM_Rect.h>

namespace ee { class MultiShapesImpl; }

namespace eshape
{

class PointShape;
class BezierShape;
class CircleShape;
class EditedPolyShape;
class RectShape;

struct NodeAddr
{
	ee::ShapePtr shape = nullptr;
	sm::vec2 pos;

	NodeAddr() 
	{
		pos.MakeInvalid();
	}

	void Clear() {
		shape = nullptr;
		pos.MakeInvalid();
	}
};

class NodeCapture
{
public:
	NodeCapture(ee::MultiShapesImpl* shapes_impl, int tol);

	void captureEditable(const sm::vec2& pos, NodeAddr& result);
	void captureSelectable(const sm::vec2& pos, NodeAddr& result);

private:
	class RectQueryVisitor : public ee::RefVisitor<ee::Shape>
	{
	public:
		RectQueryVisitor(const sm::vec2& pos, float tolerance, NodeAddr& result);

		virtual void Visit(const ee::ShapePtr& shape, bool& next) override;

	private:
		bool Visit(const std::shared_ptr<PointShape>& point);
		bool Visit(const std::shared_ptr<BezierShape>& bezier);
		bool Visit(const std::shared_ptr<EditedPolyShape>& polyline);
		bool Visit(const std::shared_ptr<CircleShape>& circle);
		bool Visit(const std::shared_ptr<RectShape>& rect);

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