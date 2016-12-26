#include "Buffer.h"

namespace sl
{

Buffer::Buffer(int stride, int cap)
	: m_stride(stride)
	, m_capacity(cap)
	, m_count(0)
	, m_dirty(false)
{
	m_buffer = new unsigned char[stride * cap];
}

Buffer::~Buffer()
{
	delete[] m_buffer;
}

}