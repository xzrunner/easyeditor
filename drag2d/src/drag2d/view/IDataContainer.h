#pragma once

#include "interfaces.h"

#include "common/Object.h"
#include "view/EditPanelDefs.h"

namespace d2d
{
	class IDataContainer : public Object
	{
	public:
		virtual void traverse(IVisitor& visitor, bool order = true) const = 0;
		virtual void traverse(IVisitor& visitor, TraverseType type = e_allExisting, bool order = true) const = 0;
		virtual void remove(Object* obj) = 0;
		virtual void insert(Object* obj) = 0;
		virtual void clear() = 0;
		virtual void resetOrder(const Object* obj, bool up) = 0;
		virtual ~IDataContainer() {}
	};
}