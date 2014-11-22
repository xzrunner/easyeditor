//#pragma once
//
//#include <vector>
//
//#include "interfaces.h"
////#include "dataset/VectorObjectStream.h"
//#include "dataset/ISprite.h"
//#include "dataset/IShape.h"
//
//namespace d2d
//{
//	template<class T>
//	class StageDataWrapper
//	{
//	public:
//		StageDataWrapper(std::vector<T*>& objs);
//		~StageDataWrapper();
//
//		void traverse(IVisitor& visitor, bool order = true) const;
//		void remove(T* obj);
//		void insert(T* obj);
//		void clear();
//
//		void resetOrder(const T* obj, bool up);
//
//// 	private:
//// 		VectorObjectStream<T> getStream() const {
//// 			return VectorObjectStream<T>(m_objects);
//// 		}
//
//	private:
//		std::vector<T*>& m_objs;
//
//	}; // StageContainer
//
//	template<class T>
//	inline StageDataWrapper<T>::StageDataWrapper(std::vector<T*>& objs) 
//		: m_objs(objs) 
//	{
//// 		for (size_t i = 0, n = m_objs.size(); i < n; ++i)
//// 			m_objs[i]->Retain();
//	}
//
//	template<class T>
//	inline StageDataWrapper<T>::~StageDataWrapper() 
//	{
////		clear();
//	}
//
//	template<class T>
//	inline void StageDataWrapper<T>::traverse(IVisitor& visitor, bool order/* = true*/) const
//	{
//		if (order)
//		{
//			std::vector<T*>::const_iterator itr = m_objs.begin();
//			for ( ; itr != m_objs.end(); ++itr)
//			{
//				bool hasNext;
//				visitor.visit(*itr, hasNext);
//				if (!hasNext) break;
//			}
//		}
//		else
//		{
//			std::vector<T*>::const_reverse_iterator itr = m_objs.rbegin();
//			for ( ; itr != m_objs.rend(); ++itr)
//			{
//				bool hasNext;
//				visitor.visit(*itr, hasNext);
//				if (!hasNext) break;
//			}
//		}
//	}
//
//	template<class T>
//	inline void StageDataWrapper<T>::remove(T* obj)
//	{
//		for (size_t i = 0, n = m_objs.size(); i < n; ++i)
//		{
//			if (m_objs[i] == obj)
//			{
//				obj->Release();
//				m_objs.erase(m_objs.begin() + i);
//				break;
//			}
//		}
//	}
//
//	template<class T>
//	inline void StageDataWrapper<T>::insert(T* obj)
//	{
//		obj->Retain();
//		m_objs.push_back(obj);
//	}
//
//	template<class T>
//	inline void StageDataWrapper<T>::clear()
//	{
//		for (size_t i = 0, n = m_objs.size(); i < n; ++i)
//			m_objs[i]->Release();
//		m_objs.clear();
//	}
//
//	template<class T>
//	inline void StageDataWrapper<T>::resetOrder(const T* obj, bool up)
//	{
//		for (size_t i = 0, n = m_objs.size(); i < n; ++i)
//		{
//			if (m_objs[i] == obj)
//			{
//				if (up && i != n - 1)
//				{
//					ISprite* tmp = m_objs[i];
//					m_objs[i] = m_objs[i+1];
//					m_objs[i+1] = tmp;
//				}
//				else if (!up && i != 0)
//				{
//					ISprite* tmp = m_objs[i];
//					m_objs[i] = m_objs[i-1];
//					m_objs[i-1] = tmp;
//				}
//
//				break;
//			}
//		}
//	}
//
//	typedef StageDataWrapper<ISprite> SpritesWrapper;
//	typedef StageDataWrapper<IShape> ShapesWrapper;
//}