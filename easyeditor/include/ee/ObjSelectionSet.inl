#ifndef _EASYEDITOR_OBJ_SELECTION_SET_INL_
#define _EASYEDITOR_OBJ_SELECTION_SET_INL_

namespace ee
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
		(*itr)->RemoveReference();
	}
	SelectionSet<T>::Clear();	
}

template<class T>
inline void ObjSelectionSet<T>::Add(T* item)
{
	if (item) {
		item->AddReference();
		SelectionSet<T>::Add(item);
	}
}

template<class T>
inline void ObjSelectionSet<T>::Remove(T* item)
{
	if (item) {
		item->RemoveReference();
		SelectionSet<T>::Remove(item);
	}
}

}

#endif // _EASYEDITOR_OBJ_SELECTION_SET_INL_