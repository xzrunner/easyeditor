#ifndef _EASYEDITOR_POINTER_POOL_H_
#define _EASYEDITOR_POINTER_POOL_H_

#include <stddef.h>

namespace ee
{

template<typename T>
class PointerPool
{
public:
	PointerPool(size_t size);	
	~PointerPool();

	T* GetPointer();

	void Reset();

private:
	PointerPool(const PointerPool<T>&);
	PointerPool<T>& operator = (const PointerPool<T>&);

private:
	T *m_start, *m_last, *m_end;

}; // PointerPool

template<typename T>
PointerPool<T>::PointerPool(size_t size)
{
	m_last = m_start = new T[size];
	m_end = m_start + size;
}

template<typename T>
PointerPool<T>::~PointerPool()
{
	if (m_start != NULL) {
		delete[] m_start;
	}
}

template<typename T>
T* PointerPool<T>::GetPointer()
{
	if (m_last == m_end) {
		return NULL;
	} else {
		return m_last++;
	}
}

template<typename T>
void PointerPool<T>::Reset()
{
	m_last = m_start;
}

}

#endif // _EASYEDITOR_POINTER_POOL_H_