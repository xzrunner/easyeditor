#pragma once

#include "interfaces.h"
#include "common/UserDataImpl.h"
#include "common/Object.h"
#include "common/Color.h"

namespace d2d
{
	class Vector;
	class Rect;

	class IShape : public ICloneable, public UserDataImpl, public Object
	{
	public:
		IShape();
		virtual ~IShape() { clearUserData(true); }

		//
		// UserDataImpl interface
		//	
		virtual void clearUserData(bool deletePtr) {}

		//
		// ICloneable interface
		//
		virtual IShape* clone() const { return NULL; }

		virtual bool isContain(const Vector& pos) const = 0;
		virtual bool isIntersect(const Rect& rect) const = 0;

		virtual const Rect& getRect() const = 0;

		virtual void draw(const Colorf& color = Colorf(0, 0, 0)) const = 0;

	protected:
		static const int QUERY_ACCURACY = 5;

	public:
		std::string name;

	}; // IShape
}

