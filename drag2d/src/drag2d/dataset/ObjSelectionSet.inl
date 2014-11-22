#ifndef _D2D_OBJ_SELECTION_SET_INL_
#define _D2D_OBJ_SELECTION_SET_INL_

namespace d2d
{

template<class T>
inline ObjSelectionSet<T>::~ObjSelectionSet()
{
	Clear();	
}

template<class T>
inline void ObjSelectionSet<T>::Clear()
{
	std::set<T*>::iterator itr = m_items.begin();
	for ( ; itr != m_items.end(); ++itr) {
		(*itr)->release();
	}
	SelectionSet<T>::Clear();	
}

template<class T>
inline void ObjSelectionSet<T>::Add(T* item)
{
	item->retain();
	SelectionSet<T>::Add(item);
}

template<class T>
inline void ObjSelectionSet<T>::Remove(T* item)
{
	item->release();
	SelectionSet<T>::Remove(item);
}

}

#endif // _D2D_OBJ_SELECTION_SET_INL_