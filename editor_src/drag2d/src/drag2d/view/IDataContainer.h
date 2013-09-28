#pragma once

#include "interfaces.h"

#include "common/Object.h"

namespace d2d
{
	class IDataContainer : public Object
	{
	public:
		virtual void traverse(IVisitor& visitor, bool order = true) const = 0;
		virtual void remove(Object* obj) = 0;
		virtual void insert(Object* obj) = 0;
		virtual void clear() = 0;
		virtual void resetOrder(const Object* obj, bool up) = 0;
		virtual ~IDataContainer() {}
	};
}