#ifndef _EASYEDITOR_SELECTION_SET_INL_
#define _EASYEDITOR_SELECTION_SET_INL_

namespace ee
{

template<class T>
inline void SelectionSet<T>::Clear()
{
	m_items.clear();
}

template<class T>
inline void SelectionSet<T>::Add(T* spr)
{
	if (spr) {
		m_items.push_back(spr);
	}
}

template<class T>
inline void SelectionSet<T>::Remove(T* spr)
{
	std::vector<T*>::iterator itr = m_items.begin();
	for ( ; itr != m_items.end(); ++itr) {
		if (*itr == spr) {
			m_items.erase(itr);
			return;
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
inline bool SelectionSet<T>::IsExist(T* spr) const
{
	for (int i = 0, n = m_items.size(); i < n; ++i) {
		if (m_items[i] == spr) {
			return true;
		}
	}
	return false;
}

template<class T>
inline void SelectionSet<T>::Traverse(Visitor<T>& visitor) const
{
	std::vector<T*>::const_iterator itr = m_items.begin();
	for ( ; itr != m_items.end(); ++itr)
	{
		bool next;
		visitor.Visit(*itr, next);
		if (!next) break;
	}
}

}

#endif // _EASYEDITOR_SELECTION_SET_INL_
