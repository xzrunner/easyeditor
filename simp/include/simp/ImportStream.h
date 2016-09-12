#ifndef _SIMP_IMPORT_STREAM_H_
#define _SIMP_IMPORT_STREAM_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class Allocator;

class ImportStream : private cu::Uncopyable
{
public:
	ImportStream(const char* data, int sz);
	~ImportStream();

	uint8_t UInt8();
	uint16_t UInt16();
	uint32_t UInt32();

	const char* String(Allocator& alloc);

	uint32_t RGBA();
	uint32_t ABGR();

	void SetCurrentID(int id) { m_current_id = id; }

	bool Empty() const { return m_size == 0; }

private:
//	Page* pack;

	const char* m_stream;
	int m_size;

	// for debug
	int m_current_id;

}; // ImportStream

}

#endif // _SIMP_IMPORT_STREAM_H_