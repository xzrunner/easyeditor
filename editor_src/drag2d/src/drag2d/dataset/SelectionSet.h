#pragma once

#include <set>

#include "dataset/ISprite.h"
#include "dataset/IShape.h"

namespace d2d
{
	class IVisitor;

	template<class T>
	class SelectionSet : public Object
	{
	public:
		virtual ~SelectionSet() {}

		size_t size() const;
		bool empty() const;

		void clear();

		bool isExist(T* item) const;
		void insert(T* item);
		void erase(T* item);

		void traverse(IVisitor& visitor) const;

	private:
		std::set<T*> m_items;

	}; // SelectionSet
	
	template<class T>
	inline size_t SelectionSet<T>::size() const
	{
		return m_items.size();
	}

	template<class T>
	inline bool SelectionSet<T>::empty() const
	{
		return m_items.empty();
	}

	template<class T>
	inline void SelectionSet<T>::clear()
	{
		m_items.clear();
	}

	template<class T>
	inline bool SelectionSet<T>::isExist(T* sprite) const
	{
		return m_items.find(sprite) != m_items.end();
	}

	template<class T>
	inline void SelectionSet<T>::insert(T* sprite)
	{
		m_items.insert(sprite);
	}

	template<class T>
	inline void SelectionSet<T>::erase(T* sprite)
	{
		m_items.erase(sprite);
	}

	template<class T>
	inline void SelectionSet<T>::traverse(IVisitor& visitor) const
	{
		std::set<T*>::const_iterator itr = m_items.begin();
		for ( ; itr != m_items.end(); ++itr)
		{
			bool hasNext;
			visitor.visit(*itr, hasNext);
			if (!hasNext) break;
		}
	}

	typedef SelectionSet<ISprite> SpriteSelection;
	typedef SelectionSet<IShape> ShapeSelection;
	typedef SelectionSet<Vector> NodeSelection;
}

