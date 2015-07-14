#ifndef _DRAG2D_RESOURCES_MGR_H_
#define _DRAG2D_RESOURCES_MGR_H_

#include <map>

namespace d2d
{

class IVisitor;

template<class T>
class ResourcesMgr
{
public:
	static ResourcesMgr* Instance();

	T* GetItem(const std::string& filename);
	void GetItem(const std::string& filename, T** old);

	void RemoveItem(const std::string& filename);

	void Clear();

	void Traverse(IVisitor& visitor) const;

	size_t Size() const {
		return m_items.size();
	}

protected:
	ResourcesMgr();
	~ResourcesMgr();

private:
	static ResourcesMgr<T>* m_instance;

	std::map<std::string, T*> m_items;

}; // ResourcesMgr

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
		itr->second->Retain();
		return itr->second;
	}
}


template<class T>
inline void ResourcesMgr<T>::GetItem(const std::string& filename, T** old)
{
	T* _new = GetItem(filename);
	// todo: ×ªÒÆµ½getItemÖÐ
//		_new->Retain();
	if (_new != *old && *old != NULL)
		(*old)->Release();
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
	std::map<std::string, T*>::iterator itr = m_items.begin();
	for ( ; itr != m_items.end(); ++itr)
		delete itr->second;
	m_items.clear();
}

template<class T>
inline void ResourcesMgr<T>::Traverse(IVisitor& visitor) const
{
	std::map<std::string, T*>::const_iterator itr = m_items.begin();
	for ( ; itr != m_items.end(); ++itr)
	{
		bool hasNext;
		visitor.Visit(itr->second, hasNext);
		if (!hasNext) break;
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

#endif // _DRAG2D_RESOURCES_MGR_H_