#ifndef _D2D_SELECTION_SET_INL_
#define _D2D_SELECTION_SET_INL_

namespace d2d
{

template<class T>
inline void SelectionSet<T>::Clear()
{
	m_items.clear();
}

template<class T>
inline void SelectionSet<T>::Add(T* sprite)
{
	if (sprite) {
		m_items.push_back(sprite);
	}
}

template<class T>
inline void SelectionSet<T>::Remove(T* sprite)
{
	std::vector<T*>::iterator itr = m_items.begin();
	for ( ; itr != m_items.end(); ++itr) {
		if (*itr == sprite) {
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
inline bool SelectionSet<T>::IsExist(T* sprite) const
{
	for (int i = 0, n = m_items.size(); i < n; ++i) {
		if (m_items[i] == sprite) {
			return true;
		}
	}
	return false;
}

template<class T>
inline void SelectionSet<T>::Traverse(IVisitor& visitor) const
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

#endif // _D2D_SELECTION_SET_INL_
