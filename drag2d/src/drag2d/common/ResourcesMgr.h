#pragma once

#include <wx/wx.h>
#include <map>

namespace d2d
{
	class IVisitor;

	template<class T>
	class ResourcesMgr
	{
	public:
		static ResourcesMgr* Instance();

		T* getItem(const wxString& filename);
		void getItem(const wxString& filename, T** old);

		void removeItem(const wxString& filename);

		void clear();

		void traverse(IVisitor& visitor) const;

		size_t size() const {
			return m_items.size();
		}

	protected:
		ResourcesMgr();
		~ResourcesMgr();

	private:
		static ResourcesMgr<T>* m_instance;

		std::map<wxString, T*> m_items;

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
	inline T* ResourcesMgr<T>::getItem(const wxString& filename)
	{
		std::map<wxString, T*>::iterator itr = m_items.find(filename);
		if (itr == m_items.end())
		{
			T* item = new T;
			bool loaded = item->loadFromFile(filename);
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
	inline void ResourcesMgr<T>::getItem(const wxString& filename, T** old)
	{
		T* _new = getItem(filename);
		// todo: ×ªÒÆµ½getItemÖÐ
//		_new->Retain();
		if (_new != *old && *old != NULL)
			(*old)->Release();
		*old = _new;
	}

	template<class T>
	inline void ResourcesMgr<T>::removeItem(const wxString& filename)
	{
		std::map<wxString, T*>::iterator itr = m_items.find(filename);
		//assert(itr != m_items.end());
		if (itr != m_items.end()) {
			m_items.erase(itr);
		}
	}

	template<class T>
	inline void ResourcesMgr<T>::clear()
	{
		std::map<wxString, T*>::iterator itr = m_items.begin();
		for ( ; itr != m_items.end(); ++itr)
			delete itr->second;
		m_items.clear();
	}

	template<class T>
	inline void ResourcesMgr<T>::traverse(IVisitor& visitor) const
	{
		std::map<wxString, T*>::const_iterator itr = m_items.begin();
		for ( ; itr != m_items.end(); ++itr)
		{
			bool hasNext;
			visitor.visit(itr->second, hasNext);
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
		clear();
	}
}

