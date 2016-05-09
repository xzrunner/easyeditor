#ifndef _EASYEDITOR_RECT_H_
#define _EASYEDITOR_RECT_H_

#include <SM_Vector.h>

#include <algorithm>

#include <float.h>

namespace ee
{

class Rect
{
public:
	float xmin, xmax;
	float ymin, ymax;

public:
	Rect() { MakeInfinite(); }
	Rect(const sm::vec2& p0, const sm::vec2& p1) {
		xmin = (std::min)(p0.x, p1.x);
		ymin = (std::min)(p0.y, p1.y);
		xmax = (std::max)(p0.x, p1.x);
		ymax = (std::max)(p0.y, p1.y);
	}
	Rect(const sm::vec2& center, float hWidth, float hHeight) {
		xmin = center.x - hWidth;
		xmax = center.x + hWidth;
		ymin = center.y - hHeight;
		ymax = center.y + hHeight;
	}
	Rect(float width, float height) {
		xmin = -width * 0.5f;
		ymin = -height * 0.5f;
		xmax = -xmin;
		ymax = -ymin;
	}

	const Rect& operator = (const Rect& aabb)
	{
		xmin = aabb.xmin;
		xmax = aabb.xmax;
		ymin = aabb.ymin;
		ymax = aabb.ymax;
		return *this;
	}

	bool IsValid() const {
		return xmin != FLT_MAX && ymin != FLT_MAX
			&& xmax != - FLT_MAX && ymax != - FLT_MAX
			&& xmin <= xmax && ymin <= ymax;
	}
	void MakeInfinite() {
		xmin = ymin = FLT_MAX;
		xmax = ymax = - FLT_MAX;
	}

	float Width() const { return xmax - xmin; }
	float Height() const { return ymax - ymin; }

	float CenterX() const { return (xmin + xmax) * 0.5f; }
	float CenterY() const { return (ymin + ymax) * 0.5f; }
	sm::vec2 Center() const { return sm::vec2(CenterX(), CenterY()); }

	void Translate(const sm::vec2& offset)
	{
		xmin += offset.x;
		xmax += offset.x;
		ymin += offset.y;
		ymax += offset.y;
	}

	void Scale(float sx, float sy)
	{
		xmin *= sx;
		xmax *= sx;
		ymin *= sy;
		ymax *= sy;
	}

	void Shear(float sx, float sy)
	{
		// x' = x + y*kx
		// y' = x*ky + y
		sm::vec2 v[4];
		v[0].x = xmin + ymin*sx;
		v[0].y = xmin*sy + ymin;
		v[1].x = xmax + ymin*sx;
		v[1].y = xmax*sy + ymin;
		v[2].x = xmax + ymax*sx;
		v[2].y = xmax*sy + ymax;
		v[3].x = xmin + ymax*sx;
		v[3].y = xmin*sy + ymax;
		for (int i = 0; i < 4; ++i)
		{
			if (v[i].x < xmin)
				xmin = v[i].x;
			else if (v[i].x > xmax)
				xmax = v[i].x;
			if (v[i].y < ymin)
				ymin = v[i].y;
			else if (v[i].y > ymax)
				ymax = v[i].y;
		}
	}

	void Combine(const sm::vec2& pos)
	{
		if (pos.x < xmin) xmin = pos.x;
		if (pos.x > xmax) xmax = pos.x;
		if (pos.y < ymin) ymin = pos.y;
		if (pos.y > ymax) ymax = pos.y;
	}
	void Combine(const Rect& aabb)
	{
		if (aabb.xmin < xmin) xmin = aabb.xmin;
		if (aabb.xmax > xmax) xmax = aabb.xmax;
		if (aabb.ymin < ymin) ymin = aabb.ymin;
		if (aabb.ymax > ymax) ymax = aabb.ymax;
	}

}; // Rect

}

#endif // _EASYEDITOR_RECT_H_