#pragma once

#include "common/Vector.h"

namespace d2d
{
	class Rect
	{
	public:
		float xMin, xMax;
		float yMin, yMax;

		Rect() { makeInfinite(); }
		Rect(const Vector& p0, const Vector& p1) {
			xMin = std::min(p0.x, p1.x);
			yMin = std::min(p0.y, p1.y);
			xMax = std::max(p0.x, p1.x);
			yMax = std::max(p0.y, p1.y);
		}
		Rect(const Vector& center, float hWidth, float hHeight) {
			xMin = center.x - hWidth;
			xMax = center.x + hWidth;
			yMin = center.y - hHeight;
			yMax = center.y + hHeight;
		}

		const Rect& operator = (const Rect& aabb)
		{
			xMin = aabb.xMin;
			xMax = aabb.xMax;
			yMin = aabb.yMin;
			yMax = aabb.yMax;
			return *this;
		}

		bool isValid() const {
			return xMin != FLT_MAX && yMin != FLT_MAX
				&& xMax != - FLT_MAX && yMax != - FLT_MAX;
		}
		void makeInfinite() {
			xMin = yMin = FLT_MAX;
			xMax = yMax = - FLT_MAX;
		}

		float xLength() const { return xMax - xMin; }
		float yLength() const { return yMax - yMin; }

		float xCenter() const { return (xMin + xMax) * 0.5f; }
		float yCenter() const { return (yMin + yMax) * 0.5f; }

		void translate(const Vector& offset)
		{
			xMin += offset.x;
			xMax += offset.x;
			yMin += offset.y;
			yMax += offset.y;
		}

		void scale(float xScale, float yScale)
		{
			const float cx = xCenter(),
				cy = yCenter();
			const float dx = xLength() * xScale * 0.5f,
				dy = yLength() * yScale * 0.5f;
			xMin = cx - dx;
			xMax = cx + dx;
			yMin = cy - dy;
			yMax = cy + dy;
		}

		void combine(const Vector& pos)
		{
			if (pos.x < xMin) xMin = pos.x;
			if (pos.x > xMax) xMax = pos.x;
			if (pos.y < yMin) yMin = pos.y;
			if (pos.y > yMax) yMax = pos.y;
		}
		void combine(const Rect& aabb)
		{
			if (aabb.xMin < xMin) xMin = aabb.xMin;
			if (aabb.xMax > xMax) xMax = aabb.xMax;
			if (aabb.yMin < yMin) yMin = aabb.yMin;
			if (aabb.yMax > yMax) yMax = aabb.yMax;
		}

	}; // Rect
}

