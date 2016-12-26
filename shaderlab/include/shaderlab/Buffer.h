#ifndef _SHADERLAB_BUFFER_H_
#define _SHADERLAB_BUFFER_H_

#include <string.h>

namespace sl
{

class Buffer
{
public:
	Buffer(int stride, int cap);
	~Buffer();

	bool IsEmpty() const { return m_count == 0; }

	bool IsDirty() const { return m_dirty; }
	void ResetDirty() { m_dirty = false; }

	void Clear() { m_count = 0; }
	int Size() const { return m_count; }
	int Capacity() const { return m_capacity; }

	const unsigned char* Data() const { return m_buffer; }

	bool Add(const void* data, int n) {
		if (m_count + n > m_capacity) {
			return true;
		} else {
			if (m_buffer && data) {
				memcpy(m_buffer + m_stride * m_count, data, m_stride * n);
			}
			m_dirty = true;
			m_count += n;
			return false;
		}
	}

private:
	unsigned char* m_buffer;

	int m_stride, m_capacity;
	int m_count;

	bool m_dirty;

}; // Buffer

}

#endif // _SHADERLAB_BUFFER_H_