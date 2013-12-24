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

		void clear();

		void traverse(IVisitor& visitor) const;

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
			return itr->second;
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

