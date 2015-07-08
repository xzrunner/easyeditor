#ifndef _DRAG2D_I_DATA_CONTAINER_H_
#define _DRAG2D_I_DATA_CONTAINER_H_

#include "interfaces.h"

#include "common/Object.h"
#include "view/DataTraverseType.h"

namespace d2d
{

class IDataContainer : public Object
{
public:
	virtual void Traverse(IVisitor& visitor, bool order = true) const = 0;
	virtual void Traverse(IVisitor& visitor, DataTraverseType type = DT_ALL, bool order = true) const = 0;
	virtual bool Remove(Object* obj) = 0;
	virtual bool Insert(Object* obj) = 0;
	virtual bool Clear() = 0;
	virtual bool ResetOrder(const Object* obj, bool up) = 0;
	virtual ~IDataContainer() {}
};

}

#endif // _DRAG2D_I_DATA_CONTAINER_H_