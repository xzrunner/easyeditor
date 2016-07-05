#ifndef _EASYEDITOR_DATA_CONTAINER_H_
#define _EASYEDITOR_DATA_CONTAINER_H_

#include "Object.h"
#include "DataTraverseType.h"

#include <vector>

namespace ee
{

class Visitor;

class DataContainer : public Object
{
public:
	virtual void Traverse(Visitor& visitor, bool order = true) const = 0;
	virtual void Traverse(Visitor& visitor, DataTraverseType type = DT_ALL, bool order = true) const = 0;
	virtual bool Remove(Object* obj) = 0;
	virtual bool Insert(Object* obj) = 0;
	virtual bool Insert(Object* obj, int idx) = 0;
	virtual bool Clear() = 0;
	virtual bool ResetOrder(const Object* obj, bool up) = 0;
	virtual bool ResetOrderMost(const Object* obj, bool up) = 0;
	virtual ~DataContainer() {}
};

}

#endif // _EASYEDITOR_DATA_CONTAINER_H_