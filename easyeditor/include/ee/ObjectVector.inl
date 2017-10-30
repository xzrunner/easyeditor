#ifndef _EASYEDITOR_VECTOR_CONTAINER_INL_
#define _EASYEDITOR_VECTOR_CONTAINER_INL_

#include "Visitor.h"

namespace ee
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
void ObjectVector<T>::Traverse(RefVisitor<T>& visitor, bool order) const
{
	Traverse(m_objs, visitor, order);
}

template<class T>
void ObjectVector<T>::Traverse(RefVisitor<T>& visitor, DataTraverseType type, bool order) const
{
	Traverse(m_objs, visitor, type, order);
}

template<class T>
bool ObjectVector<T>::Remove(const std::shared_ptr<T>& obj)
{
	return Remove(m_objs, obj);
}

template<class T>
bool ObjectVector<T>::Insert(const std::shared_ptr<T>& obj)
{
	return Insert(m_objs, obj);
}

template<class T>
bool ObjectVector<T>::Insert(const std::shared_ptr<T>& obj, int idx)
{
	return Insert(m_objs, obj, idx);
}

template<class T>
bool ObjectVector<T>::Clear()
{
	return Clear(m_objs);
}

template<class T>
bool ObjectVector<T>::ResetOrder(const std::shared_ptr<const T>& obj, bool up)
{
	return ResetOrder(m_objs, obj, up);
}

template<class T>
bool ObjectVector<T>::ResetOrderMost(const std::shared_ptr<const T>& obj, bool up)
{
	return ResetOrderMost(m_objs, obj, up);
}

template<class T>
bool ObjectVector<T>::Sort(std::vector<std::shared_ptr<T>>& list)
{
	return Sort(m_objs, list);
}

template<class T>
bool ObjectVector<T>::IsExist(const std::shared_ptr<T>& obj) const
{
	for (int i = 0, n = m_objs.size(); i < n; ++i) {
		if (obj == m_objs[i]) {
			return true;
		}
	}
	return false;
}

template<class T>
int ObjectVector<T>::Size() const
{
	return m_objs.size();
}

template<class T>
inline void ObjectVector<T>::Traverse(const std::vector<std::shared_ptr<T>>& objs, RefVisitor<T>& visitor, bool order/* = true*/)
{
	if (order)
	{
		auto itr = objs.begin();
		for ( ; itr != objs.end(); ++itr)
		{
			bool next;
			visitor.Visit(*itr, next);
			if (!next) break;
		}
	}
	else
	{
		auto itr = objs.rbegin();
		for ( ; itr != objs.rend(); ++itr)
		{
			bool next;
			visitor.Visit(*itr, next);
			if (!next) break;
		}
	}
}

template<class T>
inline void ObjectVector<T>::Traverse(const std::vector<std::shared_ptr<T>>& objs,
									  RefVisitor<T>& visitor, 
									  DataTraverseType type, 
									  bool order)
{
	if (order)
	{
		auto itr = objs.begin();
		for ( ; itr != objs.end(); ++itr)
		{
			if (type == DT_EDITABLE && (*itr)->IsEditable() ||
				type == DT_VISIBLE && (*itr)->IsVisible() ||
				type == DT_ALL || type == DT_SELECTABLE)
			{
				bool next;
				visitor.Visit(*itr, next);
				if (!next) break;
			}
		}
	}
	else
	{
		auto itr = objs.rbegin();
		for ( ; itr != objs.rend(); ++itr)
		{
			bool next;
			visitor.Visit(*itr, next);
			if (!next) break;
		}
	}
}

template<class T>
inline bool ObjectVector<T>::Remove(std::vector<std::shared_ptr<T>>& objs, 
									const std::shared_ptr<T>& obj)
{
	for (size_t i = 0, n = objs.size(); i < n; ++i)
	{
		if (objs[i] == obj)
		{
			objs.erase(objs.begin() + i);
			return true;
		}
	}
	return false;
}

template<class T>
inline bool ObjectVector<T>::Insert(std::vector<std::shared_ptr<T>>& objs, 
									const std::shared_ptr<T>& obj)
{
	objs.push_back(obj);
	return true;
}

template<class T>
inline bool ObjectVector<T>::Insert(std::vector<std::shared_ptr<T>>& objs, const std::shared_ptr<T>& obj, int idx)
{
	if (objs.empty() || idx >= (int)objs.size()) {
		objs.push_back(obj);
	} else if (idx < 0) {
		objs.insert(objs.begin(), obj);
	} else {
		objs.insert(objs.begin() + idx, obj);
	}
	return true;
}

template<class T>
inline bool ObjectVector<T>::Clear(std::vector<std::shared_ptr<T>>& objs)
{
	bool ret = !objs.empty();
	objs.clear();
	return ret;
}

template<class T>
inline bool ObjectVector<T>::ResetOrder(std::vector<std::shared_ptr<T>>& objs, 
										const std::shared_ptr<const T>& obj,
										bool up)
{
	for (size_t i = 0, n = objs.size(); i < n; ++i)
	{
		if (objs[i] == obj)
		{
			if (up && i != n - 1)
			{
				std::swap(objs[i], objs[i + 1]);
				return true;
			}
			else if (!up && i != 0)
			{
				std::swap(objs[i], objs[i - 1]);
				return true;
			}
			return false;
		}
	}
	return false;
}

template<class T>
inline bool ObjectVector<T>::ResetOrderMost(std::vector<std::shared_ptr<T>>& objs, const std::shared_ptr<const T>& obj, bool up) {
	for (int i = 0, n = objs.size(); i < n; ++i) {
		if (objs[i] != obj) {
			continue;
		}
		if (up && i != n - 1) {
			auto tmp = objs[i];
			for (int j = i + 1; j < n; ++j) {
				objs[j-1] = objs[j];
			}
			objs[n - 1] = tmp;
			return true;
		} else if (!up && i != 0) {
			auto tmp = objs[i];
			for (int j = i - 1; j >= 0; --j) {
				objs[j+1] = objs[j];
			}
			objs[0] = tmp;
			return true;
		}
		return false;
	}
	return false;
}

template<class T>
inline bool ObjectVector<T>::Sort(std::vector<std::shared_ptr<T>>& objs, std::vector<std::shared_ptr<T>>& list) {
	std::map<int, std::shared_ptr<T>> order_sorted;
	for (int i = 0, n = list.size(); i < n; ++i) {
		const std::shared_ptr<T>& obj = list[i];
		for (int j = 0, m = objs.size(); j < m; ++j) {
			if (obj == objs[j]) {
				order_sorted.insert(std::make_pair(j, obj));
			}
		}
	}

	if (order_sorted.size() != list.size()) {
		return false;
	}
	
	std::vector<std::shared_ptr<T>> list_dst;
	list_dst.reserve(list.size());
	auto itr = order_sorted.begin();
	for ( ; itr != order_sorted.end(); ++itr) {
		list_dst.push_back(itr->second);
	}
	list = list_dst;
	return true;
}

}

#endif // _EASYEDITOR_VECTOR_CONTAINER_INL_