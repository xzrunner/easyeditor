#include "ImportStream.h"
#include "Allocator.h"
#include "simp_define.h"

#include <fault.h>

namespace simp
{

ImportStream::ImportStream(const char* data, int sz)
	: m_stream(data)
	, m_size(sz)
	, m_current_id(-1)
{
}

ImportStream::~ImportStream()
{
}

uint8_t ImportStream::UInt8()
{
	if (m_size == 0) {
		fault("Invalid import stream (%d)", m_current_id);
	}
	uint8_t ret = (uint8_t)*(m_stream);
	++m_stream;
	--m_size;
	return ret;	
}

uint16_t ImportStream::UInt16()
{
	if (m_size < 2) {
		fault("Invalid import stream (%d)", m_current_id);
	}
	uint8_t low = (uint8_t)*(m_stream);
	uint8_t high = (uint8_t)*(m_stream + 1);
	m_stream += 2;
	m_size -= 2;
	return low | (uint32_t)high << 8;
}

uint32_t ImportStream::UInt32()
{
	if (m_size < 4) {
		fault("Invalid import stream (%d)", m_current_id);
	}

	uint8_t b[4];
	b[0] = (uint8_t)*(m_stream);
	b[1] = (uint8_t)*(m_stream + 1);
	b[2] = (uint8_t)*(m_stream + 2);
	b[3] = (uint8_t)*(m_stream + 3);
	m_stream += 4;
	m_size -= 4;

	return b[0] | (uint32_t)b[1] << 8 | (uint32_t)b[2] << 16 | (uint32_t)b[3] << 24;
}

const char* ImportStream::Block(int sz)
{
	if (m_size < sz) {
		fault("Invalid import stream (%d)", m_current_id);
	}

	const char* ret = m_stream;
	m_stream += sz;
	m_size -= sz;

	return ret;
}

const char* ImportStream::String(Allocator& alloc)
{
	int n = UInt8();
	if (n == 255) {
		return NULL;
	}
	if (m_size < n) {
		fault("Invalid stream (%d): read string failed", m_current_id);
	}
	char* buf = static_cast<char*>(alloc.Alloc(ALIGN_4BYTE(n)));
	memcpy(buf, m_stream, n);
	buf[n] = 0;
	m_stream += n;
	m_size -= n;

	return buf;
}

std::string ImportStream::String()
{
	int n = UInt8();
	if (n == 255) {
		return NULL;
	}
	if (m_size < n) {
		fault("Invalid stream (%d): read string failed", m_current_id);
	}

	std::string str(m_stream, n);
	m_stream += n;
	m_size -= n;

	return str;
}

uint32_t ImportStream::RGBA()
{
	if (m_size < 4) {
		fault("Invalid import stream (%d)", m_current_id);
	}

	uint8_t b[4];
	b[0] = (uint8_t)*(m_stream);
	b[1] = (uint8_t)*(m_stream + 1);
	b[2] = (uint8_t)*(m_stream + 2);
	b[3] = (uint8_t)*(m_stream + 3);
	m_stream += 4;
	m_size -= 4;

	return b[0] | (uint32_t)b[1] << 8 | (uint32_t)b[2] << 16 | (uint32_t)b[3] << 24;
}

uint32_t ImportStream::ABGR()
{
	if (m_size < 4) {
		fault("Invalid import stream (%d)", m_current_id);
	}

	uint8_t b[4];
	b[0] = (uint8_t)*(m_stream);
	b[1] = (uint8_t)*(m_stream + 1);
	b[2] = (uint8_t)*(m_stream + 2);
	b[3] = (uint8_t)*(m_stream + 3);
	m_stream += 4;
	m_size -= 4;

	return b[3] | (uint32_t)b[2] << 8 | (uint32_t)b[1] << 16 | (uint32_t)b[0] << 24;
}

}