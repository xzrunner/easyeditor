#pragma once

namespace ee0
{

template<class T>
inline void SelectionSet<T>::Clear()
{
	m_items.clear();
}

template<class T>
inline void SelectionSet<T>::Add(const std::shared_ptr<T>& spr)
{
	if (spr) {
		m_items.push_back(spr);
	}
}

template<class T>
inline void SelectionSet<T>::Remove(const std::shared_ptr<T>& spr)
{
	auto itr = m_items.begin();
	for (; itr != m_items.end(); ++itr) {
		if (*itr == spr) {
			m_items.erase(itr);
			break;
		}
	}
}

template<class T>
inline size_t SelectionSet<T>::Size() const
{
	return m_items.size();
}

template<class T>
inline bool SelectionSet<T>::IsEmpty() const
{
	return m_items.empty();
}

template<class T>
inline bool SelectionSet<T>::IsExist(const std::shared_ptr<T>& spr) const
{
	for (auto& item : m_items) {
		if (item == spr) {
			return true;
		}
	}
	return false;
}

template<class T>
inline void SelectionSet<T>::Traverse(std::function<bool(const std::shared_ptr<T>&)> func) const
{
	for (auto& item : m_items) {
		if (!func(item)) {
			break;
		}
	}
}

}