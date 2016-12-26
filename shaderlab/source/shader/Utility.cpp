#include "Utility.h"
#include "StackAllocator.h"
#include "Buffer.h"
#include "RenderBuffer.h"

#include <unirender/typedef.h>

namespace sl
{

RenderBuffer* Utility::CreateIndexBuffer(ur::RenderContext* rc, int count)
{
	StackAllocator* alloc = StackAllocator::Instance();
	int sz = sizeof(uint16_t) * count;
	alloc->Reserve(sz);
	void* buf = alloc->Alloc(sz);
	memset(buf, 0, sz);
	Buffer* index_buf = new Buffer(sizeof(uint16_t), count);
	index_buf->Add(buf, count);
	alloc->Free(buf);
	RenderBuffer* ret = new RenderBuffer(rc, ur::INDEXBUFFER, sizeof(uint16_t), count, index_buf);	
	ret->Update();
    ret->Clear();
	return ret;	
}

RenderBuffer* Utility::CreateQuadIndexBuffer(ur::RenderContext* rc, int quad_count)
{
	StackAllocator* alloc = StackAllocator::Instance();
	int sz = sizeof(uint16_t) * 6 * quad_count;
	alloc->Reserve(sz);
	void* buf = alloc->Alloc(sz);
	uint16_t* ptr = (uint16_t*)buf;
	FillingQuadIndexBuffer(ptr, quad_count);
	Buffer* index_buf = new Buffer(sizeof(uint16_t), 6 * quad_count);
	index_buf->Add(buf, 6 * quad_count);
	alloc->Free(buf);
	RenderBuffer* ret = new RenderBuffer(rc, ur::INDEXBUFFER, sizeof(uint16_t), 6 * quad_count, index_buf);	
	ret->Update();
    ret->Clear();
	return ret;
}

void Utility::FillingQuadIndexBuffer(uint16_t* buf, int quad_count)
{
	for (int i = 0; i < quad_count; ++i) 
	{
		buf[i*6]   = i*4;
		buf[i*6+1] = i*4+1;
		buf[i*6+2] = i*4+2;
		buf[i*6+3] = i*4;
		buf[i*6+4] = i*4+2;
		buf[i*6+5] = i*4+3;
	}
}

}