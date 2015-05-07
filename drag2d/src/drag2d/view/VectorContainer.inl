#ifndef _DRAG2D_VECTOR_CONTAINER_INL_
#define _DRAG2D_VECTOR_CONTAINER_INL_

namespace d2d
{

template<class T>
inline void VectorContainer::Traverse(const std::vector<T*>& objs, 
									  IVisitor& visitor, 
									  bool order/* = true*/)
{
	if (order)
	{
		std::vector<T*>::const_iterator itr = objs.begin();
		for ( ; itr != objs.end(); ++itr)
		{
			bool hasNext;
			visitor.visit(*itr, hasNext);
			if (!hasNext) break;
		}
	}
	else
	{
		std::vector<T*>::const_reverse_iterator itr = objs.rbegin();
		for ( ; itr != objs.rend(); ++itr)
		{
			bool hasNext;
			visitor.visit(*itr, hasNext);
			if (!hasNext) break;
		}
	}
}

template<class T>
inline void VectorContainer::Traverse(const std::vector<T*>& objs,
									  IVisitor& visitor, 
									  DataTraverseType type, 
									  bool order)
{
	if (order)
	{
		std::vector<T*>::const_iterator itr = objs.begin();
		for ( ; itr != objs.end(); ++itr)
		{
			if (type == DT_EDITABLE && (*itr)->editable ||
				type == DT_VISIBLE && (*itr)->visiable ||
				type == DT_ALL || type == DT_SELECTABLE)
			{
				bool hasNext;
				visitor.visit(*itr, hasNext);
				if (!hasNext) break;
			}
		}
	}
	else
	{
		std::vector<T*>::const_reverse_iterator itr = objs.rbegin();
		for ( ; itr != objs.rend(); ++itr)
		{
			bool hasNext;
			visitor.visit(*itr, hasNext);
			if (!hasNext) break;
		}
	}
}

template<class T>
inline bool VectorContainer::Remove(std::vector<T*>& objs, 
									T* obj)
{
	for (size_t i = 0, n = objs.size(); i < n; ++i)
	{
		if (objs[i] == obj)
		{
			obj->Release();
			objs.erase(objs.begin() + i);
			return true;
		}
	}
	return false;
}

template<class T>
inline void VectorContainer::Insert(std::vector<T*>& objs, 
									T* obj)
{
	obj->Retain();
	objs.push_back(obj);
}

template<class T>
inline void VectorContainer::Clear(std::vector<T*>& objs)
{
	for (size_t i = 0, n = objs.size(); i < n; ++i)
		objs[i]->Release();
	objs.clear();
}

template<class T>
inline bool VectorContainer::ResetOrder(std::vector<T*>& objs, 
										const T* obj, 
										bool up)
{
	for (size_t i = 0, n = objs.size(); i < n; ++i)
	{
		if (objs[i] == obj)
		{
			if (up && i != n - 1)
			{
				T* tmp = objs[i];
				objs[i] = objs[i+1];
				objs[i+1] = tmp;
			}
			else if (!up && i != 0)
			{
				T* tmp = objs[i];
				objs[i] = objs[i-1];
				objs[i-1] = tmp;
			}

			return true;
		}
	}
	return false;
}

}

#endif // _DRAG2D_VECTOR_CONTAINER_INL_