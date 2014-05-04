#pragma once

#include "IShape.h"

#include "common/Vector.h"
#include "common/Rect.h"

namespace d2d
{
	class CircleShape : public IShape
	{
	public:
		CircleShape();
		CircleShape(const CircleShape& circle);
		CircleShape(const Vector& center, float radius);

		//
		// IObject interface
		//
		virtual CircleShape* clone() const;

		//
		// IShape interface
		//
		virtual bool isContain(const Vector& pos) const;
		virtual bool isIntersect(const Rect& rect) const;
		virtual const Rect& getRect() const { return m_rect; }
		virtual void draw(const Colorf& color = Colorf(0, 0, 0)) const;

	public:
		Vector center;

		float radius;

	private:
		Rect m_rect;

	}; // CircleShape
}

