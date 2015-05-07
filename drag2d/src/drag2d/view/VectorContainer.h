#ifndef _DRAG2D_VECTOR_CONTAINER_H_
#define _DRAG2D_VECTOR_CONTAINER_H_

#include <vector>

#include "interfaces.h"

#include "view/DataTraverseType.h"

namespace d2d
{

class VectorContainer
{
public:
	template<class T>
	static void Traverse(const std::vector<T*>& objs, IVisitor& visitor, 
		bool order = true);
	template<class T>
	static void Traverse(const std::vector<T*>& objs, IVisitor& visitor, 
		DataTraverseType type = DT_ALL, bool order = true);

	template<class T>
	static bool Remove(std::vector<T*>& objs, T* obj);

	template<class T>
	static void Insert(std::vector<T*>& objs, T* obj);

	template<class T>
	static void Clear(std::vector<T*>& objs);

	template<class T>
	static bool ResetOrder(std::vector<T*>& objs, const T* obj, bool up);

}; // VectorContainer

}

#include "VectorContainer.inl"

#endif // _DRAG2D_VECTOR_CONTAINER_H_