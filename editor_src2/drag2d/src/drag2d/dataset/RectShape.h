#pragma once

#include "IShape.h"

#include "common/Rect.h"
#include "render/ShapeStyle.h"

namespace d2d
{
	class RectShape : public IShape
	{
	public:
		RectShape();
		RectShape(const RectShape& rect);
		RectShape(const Vector& p0, const Vector& p1);
		RectShape(const Vector& center, float hWidth, float hHeight);

		//
		// IObject interface
		//
		virtual RectShape* clone() const;

		//
		// IShape interface
		//
		virtual bool isContain(const Vector& pos) const;
		virtual bool isIntersect(const Rect& rect) const;
		virtual const Rect& getRect() const { return m_rect; }
		virtual void draw(const Colorf& color = Colorf(0, 0, 0)) const;

	public:
		Rect m_rect;

	private:
		ShapeStyle m_style;

	}; // RectShape
}

