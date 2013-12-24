#pragma once

#include <interfaces.h>

namespace d2d
{
	class Object;

	template<class T>
	class VectorObjectStream : public IObjectStream
	{
	public:
		VectorObjectStream(const std::vector<T*>& objects)
			: m_next(0), m_objects(objects) {}

		//
		// IObjectStream interface
		//
		virtual Object* getNext() {
			if (m_next >= m_objects.size())
				return NULL;				
			return m_objects[m_next++];
		}
		virtual bool hasNext() const {
			return m_next < m_objects.size();
		}
		virtual void rewind() {
			m_next = 0;
		}

	private:
		size_t m_next;
		const std::vector<T*>& m_objects;

	}; // VectorObjectStream
}