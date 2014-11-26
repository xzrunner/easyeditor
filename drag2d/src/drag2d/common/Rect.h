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
		Rect(float width, float height) {
			xMin = -width * 0.5f;
			yMin = -height * 0.5f;
			xMax = -xMin;
			yMax = -yMin;
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
				&& xMax != - FLT_MAX && yMax != - FLT_MAX
				&& xMin <= xMax && yMin <= yMax;
		}
		void makeInfinite() {
			xMin = yMin = FLT_MAX;
			xMax = yMax = - FLT_MAX;
		}

		float xLength() const { return xMax - xMin; }
		float yLength() const { return yMax - yMin; }

		float xCenter() const { return (xMin + xMax) * 0.5f; }
		float yCenter() const { return (yMin + yMax) * 0.5f; }
		Vector Center() const { return Vector(xCenter(), yCenter()); }

		void translate(const Vector& offset)
		{
			xMin += offset.x;
			xMax += offset.x;
			yMin += offset.y;
			yMax += offset.y;
		}

		void scale(float xScale, float yScale)
		{
			xMin *= xScale;
			xMax *= xScale;
			yMin *= yScale;
			yMax *= yScale;
		}

		void shear(float xShear, float yShear)
		{
			// x' = x + y*kx
			// y' = x*ky + y
			Vector v[4];
			v[0].x = xMin + yMin*xShear;
			v[0].y = xMin*yShear + yMin;
			v[1].x = xMax + yMin*xShear;
			v[1].y = xMax*yShear + yMin;
			v[2].x = xMax + yMax*xShear;
			v[2].y = xMax*yShear + yMax;
			v[3].x = xMin + yMax*xShear;
			v[3].y = xMin*yShear + yMax;
			for (int i = 0; i < 4; ++i)
			{
				if (v[i].x < xMin)
					xMin = v[i].x;
				else if (v[i].x > xMax)
					xMax = v[i].x;
				if (v[i].y < yMin)
					yMin = v[i].y;
				else if (v[i].y > yMax)
					yMax = v[i].y;
			}
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

