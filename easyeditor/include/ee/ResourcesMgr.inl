#ifndef _EASYEDITOR_RESOURCES_MGR_INL_
#define _EASYEDITOR_RESOURCES_MGR_INL_

namespace ee
{

template<class T>
ResourcesMgr<T>* ResourcesMgr<T>::m_instance = NULL;

template<class T>
inline ResourcesMgr<T>* ResourcesMgr<T>::Instance()
{
	if (!m_instance)
	{
		m_instance = new ResourcesMgr<T>;
	}
	return m_instance;
}

template<class T>
inline T* ResourcesMgr<T>::GetItem(const std::string& filename)
{
	std::map<std::string, T*>::iterator itr = m_items.find(filename);
	if (itr == m_items.end())
	{
		T* item = new T;
		bool loaded = item->LoadFromFile(filename);
		if (loaded)
		{
			item->AddReference();
			m_items.insert(std::make_pair(filename, item));
			return item;
		}
		else
		{
			delete item;
			return NULL;
		}
	}
	else
	{
		itr->second->AddReference();
		return itr->second;
	}
}


template<class T>
inline void ResourcesMgr<T>::GetItem(const std::string& filename, T** old)
{
	T* _new = GetItem(filename);
	// todo: ×ªÒÆµ½getItemÖÐ
//		_new->AddReference();
	if (_new != *old && *old != NULL)
		(*old)->RemoveReference();
	*old = _new;
}

template<class T>
inline void ResourcesMgr<T>::RemoveItem(const std::string& filename)
{
	std::map<std::string, T*>::iterator itr = m_items.find(filename);
	//assert(itr != m_items.end());
	if (itr != m_items.end()) {
		m_items.erase(itr);
	}
}

template<class T>
inline void ResourcesMgr<T>::Clear()
{
	std::vector<T*> items;
	std::map<std::string, T*>::iterator itr = m_items.begin();
	for ( ; itr != m_items.end(); ++itr) {
		items.push_back(itr->second);
	}
	for (int i = 0, n = items.size(); i < n; ++i) {
		delete items[i];
	}
	m_items.clear();
}

template<class T>
inline void ResourcesMgr<T>::Traverse(Visitor<T>& visitor) const
{
	std::map<std::string, T*>::const_iterator itr = m_items.begin();
	for ( ; itr != m_items.end(); ++itr)
	{
		bool next;
		visitor.Visit(itr->second, next);
		if (!next) break;
	}
}

template<class T>
ResourcesMgr<T>::ResourcesMgr()
{
}

template<class T>
ResourcesMgr<T>::~ResourcesMgr()
{
	Clear();
}

}

#endif // _EASYEDITOR_RESOURCES_MGR_INL_
