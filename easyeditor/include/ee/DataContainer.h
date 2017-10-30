#ifndef _EASYEDITOR_DATA_CONTAINER_H_
#define _EASYEDITOR_DATA_CONTAINER_H_

#include "DataTraverseType.h"
#include "Visitor.h"

#include <vector>

namespace ee
{

template<class T>
class DataContainer
{
public:
	virtual void Traverse(RefVisitor<T>& visitor, bool order = true) const = 0;
	virtual void Traverse(RefVisitor<T>& visitor, DataTraverseType type = DT_ALL, bool order = true) const = 0;
	virtual bool Remove(const std::shared_ptr<T>& obj) = 0;
	virtual bool Insert(const std::shared_ptr<T>& obj) = 0;
	virtual bool Insert(const std::shared_ptr<T>& obj, int idx) = 0;
	virtual bool Clear() = 0;
	virtual bool ResetOrder(const std::shared_ptr<const T>& obj, bool up) = 0;
	virtual bool ResetOrderMost(const std::shared_ptr<const T>& obj, bool up) = 0;
	virtual ~DataContainer() {}
};

}

#endif // _EASYEDITOR_DATA_CONTAINER_H_