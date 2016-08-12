#ifndef _EASYEDITOR_DATA_CONTAINER_H_
#define _EASYEDITOR_DATA_CONTAINER_H_

#include "DataTraverseType.h"
#include "Visitor.h"

#include <CU_RefCountObj.h>

#include <vector>

namespace ee
{

template<class T>
class DataContainer : public cu::RefCountObj
{
public:
	virtual void Traverse(Visitor<T>& visitor, bool order = true) const = 0;
	virtual void Traverse(Visitor<T>& visitor, DataTraverseType type = DT_ALL, bool order = true) const = 0;
	virtual bool Remove(T* obj) = 0;
	virtual bool Insert(T* obj) = 0;
	virtual bool Insert(T* obj, int idx) = 0;
	virtual bool Clear() = 0;
	virtual bool ResetOrder(const T* obj, bool up) = 0;
	virtual bool ResetOrderMost(const T* obj, bool up) = 0;
	virtual ~DataContainer() {}
};

}

#endif // _EASYEDITOR_DATA_CONTAINER_H_