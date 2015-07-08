#ifndef _DRAG2D_VECTOR_CONTAINER_INL_
#define _DRAG2D_VECTOR_CONTAINER_INL_

namespace d2d
{

template<class T>
inline ObjectVector<T>::ObjectVector()
{
}

template<class T>
inline ObjectVector<T>::~ObjectVector()
{
	Clear();
}

template<class T>
void ObjectVector<T>::Traverse(IVisitor& visitor, bool order) const
{
	Traverse(m_objs, visitor, order);
}

template<class T>
void ObjectVector<T>::Traverse(IVisitor& visitor, DataTraverseType type, bool order) const
{
	Traverse(m_objs, visitor, type, order);
}

template<class T>
bool ObjectVector<T>::Remove(T* obj)
{
	return Remove(m_objs, obj);
}

template<class T>
bool ObjectVector<T>::Insert(T* obj)
{
	return Insert(m_objs, obj);
}

template<class T>
bool ObjectVector<T>::Clear()
{
	return Clear(m_objs);
}

template<class T>
bool ObjectVector<T>::ResetOrder(const T* obj, bool up)
{
	return ResetOrder(m_objs, obj, up);
}

template<class T>
bool ObjectVector<T>::IsExist(T* obj) const
{
	for (int i = 0, n = m_objs.size(); i < n; ++i) {
		if (obj == m_objs[i]) {
			return true;
		}
	}
	return false;
}

template<class T>
inline void ObjectVector<T>::Traverse(const std::vector<T*>& objs, IVisitor& visitor, bool order/* = true*/)
{
	if (order)
	{
		std::vector<T*>::const_iterator itr = objs.begin();
		for ( ; itr != objs.end(); ++itr)
		{
			bool hasNext;
			visitor.Visit(*itr, hasNext);
			if (!hasNext) break;
		}
	}
	else
	{
		std::vector<T*>::const_reverse_iterator itr = objs.rbegin();
		for ( ; itr != objs.rend(); ++itr)
		{
			bool hasNext;
			visitor.Visit(*itr, hasNext);
			if (!hasNext) break;
		}
	}
}

template<class T>
inline void ObjectVector<T>::Traverse(const std::vector<T*>& objs,
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
				visitor.Visit(*itr, hasNext);
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
			visitor.Visit(*itr, hasNext);
			if (!hasNext) break;
		}
	}
}

template<class T>
inline bool ObjectVector<T>::Remove(std::vector<T*>& objs, 
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
inline bool ObjectVector<T>::Insert(std::vector<T*>& objs, 
									T* obj)
{
	obj->Retain();
	objs.push_back(obj);
	return true;
}

template<class T>
inline bool ObjectVector<T>::Clear(std::vector<T*>& objs)
{
	bool ret = !objs.empty();
	for (size_t i = 0, n = objs.size(); i < n; ++i)
		objs[i]->Release();
	objs.clear();
	return ret;
}

template<class T>
inline bool ObjectVector<T>::ResetOrder(std::vector<T*>& objs, 
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
				return true;
			}
			else if (!up && i != 0)
			{
				T* tmp = objs[i];
				objs[i] = objs[i-1];
				objs[i-1] = tmp;
				return true;
			}
			return false;
		}
	}
	return false;
}

}

#endif // _DRAG2D_VECTOR_CONTAINER_INL_