#pragma once

#include <vector>

#include "interfaces.h"

namespace d2d
{
	class Rect;
	class Vector;

	class AbstractBV : public ICloneable, public ISerializable
	{
	public:
		
		//
		// ICloneable interface
		//
		virtual AbstractBV* clone() const { return NULL; }

		virtual void initFromRect(const Rect& rect) = 0;

		virtual bool isValid() const = 0;
		virtual void makeInfinite() = 0;

		virtual void translate(const Vector& offset) = 0;
		virtual void rotate(float delta) = 0;

		virtual void combine(const Vector& pos) = 0;
		virtual void combine(const Rect& rect) = 0;

		virtual bool isContain(const Vector& pos) const = 0;
		virtual bool isContain(const Rect& rect) const = 0;
		virtual bool isIntersect(const Rect& rect) const = 0;

		virtual float area() const = 0;
		virtual float width() const = 0;
		virtual float height() const = 0;
		virtual Vector center() const = 0;

		virtual void getBoundPos(std::vector<Vector>& bound) const = 0;

	}; // AbstractBV
}

