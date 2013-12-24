#pragma once

#include "ChainShape.h"

namespace d2d
{
	class BezierShape : public ChainShape
	{
	public:
		BezierShape(const BezierShape& bezier);
		BezierShape(const d2d::Vector points[4]);
		BezierShape(const d2d::Vector& start, const d2d::Vector& end);
		virtual ~BezierShape();

		//
		// IObject interface
		//
		virtual BezierShape* clone();

		//
		// IShape interface
		//
		virtual bool isContain(const Vector& pos) const;
		virtual void draw(const Colorf& color = Colorf(0, 0, 0)) const;

		void createCurve();

	private:
		d2d::Vector pointOnCubicBezier(float t);

	private:
		static const int RADIUS = 5;

	public:
		// [0] start, [1] ctrl1, [2] ctrl2, [3] end
		d2d::Vector points[4];

	}; // BezierShape
}

