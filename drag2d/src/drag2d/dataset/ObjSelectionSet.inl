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
	std::vector<T*>::iterator itr = m_items.begin();
	for ( ; itr != m_items.end(); ++itr) {
		(*itr)->Release();
	}
	SelectionSet<T>::Clear();	
}

template<class T>
inline void ObjSelectionSet<T>::Add(T* item)
{
	if (item) {
		item->Retain();
		SelectionSet<T>::Add(item);
	}
}

template<class T>
inline void ObjSelectionSet<T>::Remove(T* item)
{
	if (item) {
		item->Release();
		SelectionSet<T>::Remove(item);
	}
}

}

#endif // _D2D_OBJ_SELECTION_SET_INL_