#pragma once

#include "interfaces.h"
#include "common/Vector.h"
#include "interfaces.h"
#include "common/Rect.h"

namespace d2d
{
	class IShape;
	class BezierShape;
	class ChainShape;
	class CircleShape;
	class RectShape;
	class MultiShapesImpl;

	struct NodeAddr
	{
		IShape* shape;
		Vector pos;

		NodeAddr() : shape(NULL) {}

		void clear() {
			shape = NULL;
			pos.setInvalid();
		}
	};

	class NodeCapture
	{
	public:
		NodeCapture(MultiShapesImpl* shapesImpl, int tol);

		void captureEditable(const Vector& pos, NodeAddr& result);
		void captureSelectable(const Vector& pos, NodeAddr& result);

	private:
		class RectQueryVisitor : public IVisitor
		{
		public:
			RectQueryVisitor(const Vector& pos, float tolerance, NodeAddr& result);

			virtual void visit(Object* object, bool& bFetchNext);

		private:
			bool visit(BezierShape* bezier);
			bool visit(ChainShape* chain);
			bool visit(CircleShape* circle);
			bool visit(RectShape* rect);

		private:
			const Vector& m_pos;
			float m_tolerance;

			Rect m_rect;
			NodeAddr& m_result;

		}; // RectQueryVisitor

	private:
		MultiShapesImpl* m_shapesImpl;
		int m_tol;

	}; // NodeCapture
}

