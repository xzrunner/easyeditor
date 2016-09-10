#ifndef _GUM_RESOURCE_MANAGER_INL_
#define _GUM_RESOURCE_MANAGER_INL_

namespace gum
{

template<class T>
ResourceManager<T>* ResourceManager<T>::m_instance = NULL;

template<class T>
inline ResourceManager<T>* ResourceManager<T>::Instance()
{
	if (!m_instance)
	{
		m_instance = new ResourceManager<T>;
	}
	return m_instance;
}

template<class T>
inline ResourceManager<T>::ResourceManager()
{
}

template<class T>
inline ResourceManager<T>::~ResourceManager()
{
	Clear();
}

template<class T>
inline T* ResourceManager<T>::Create(const std::string& filepath)
{
	std::map<std::string, T*>::iterator itr = m_res_map.find(filepath);
	if (itr == m_res_map.end())
	{
		T* res = new T;
		bool loaded = res->LoadFromFile(filepath);
		if (loaded)
		{
			m_res_map.insert(std::make_pair(filepath, res));
			return res;
		}
		else
		{
			delete res;
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
inline void ResourceManager<T>::Release(const std::string& filepath)
{
	std::map<std::string, T*>::iterator itr = m_res_map.find(filepath);
	assert(itr != m_res_map.end());
	if (itr != m_res_map.end()) {
		itr->second->RemoveReference();
		m_res_map.erase(itr->second);
	}
}

template<class T>
inline void ResourceManager<T>::Clear()
{
	std::map<std::string, T*>::iterator itr = m_res_map.begin();
	for ( ; itr != m_res_map.end(); ++itr) {
		itr->second->RemoveReference();
	}
	m_res_map.clear();
}

}

#endif // _GUM_RESOURCE_MANAGER_INL_
