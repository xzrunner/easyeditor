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
	m_items.insert(sprite);
}

template<class T>
inline void SelectionSet<T>::Remove(T* sprite)
{
	m_items.erase(sprite);
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
	return m_items.find(sprite) != m_items.end();
}

template<class T>
inline void SelectionSet<T>::Traverse(IVisitor& visitor) const
{
	std::set<T*>::const_iterator itr = m_items.begin();
	for ( ; itr != m_items.end(); ++itr)
	{
		bool hasNext;
		visitor.visit(*itr, hasNext);
		if (!hasNext) break;
	}
}

}

#endif // _D2D_SELECTION_SET_INL_
