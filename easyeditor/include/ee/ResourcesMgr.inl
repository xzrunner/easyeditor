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
inline std::shared_ptr<T> ResourcesMgr<T>::GetItem(const std::string& filename)
{
	auto itr = m_items.find(filename);
	if (itr == m_items.end())
	{
		auto item = std::make_shared<T>();
		bool loaded = item->LoadFromFile(filename);
		if (loaded)
		{
			m_items.insert(std::make_pair(filename, item));
			return item;
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return itr->second.lock();
	}
}

template<class T>
inline void ResourcesMgr<T>::RemoveItem(const std::string& filename)
{
	auto itr = m_items.find(filename);
	//assert(itr != m_items.end());
	if (itr != m_items.end()) {
		m_items.erase(itr);
	}
}

template<class T>
inline void ResourcesMgr<T>::Clear()
{
	m_items.clear();
}

template<class T>
inline void ResourcesMgr<T>::Traverse(RefVisitor<T>& visitor) const
{
	for (auto& item : m_items)
	{
		std::shared_ptr<T> res = item.second.lock();
		if (!res) {
			continue;
		}
		bool next;
		visitor.Visit(res, next);
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
