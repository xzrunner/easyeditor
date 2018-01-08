#include "Lzma.h"

#include <LzmaEnc.h>
#include <LzmaDec.h>
#include <Alloc.h>
#include <string>
#include <assert.h>

namespace epbin
{

static void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
static void SzFree(void *p, void *address) { p = p; MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };

static const unsigned PropHeaderSize = 5;
static const unsigned HeaderSize = 9;

// struct Block 
// {
// 	uint8_t size[4];
// 	uint8_t prop[LZMA_PROPS_SIZE];
// 	uint8_t data[119];
// };

void Lzma::Compress(unsigned char** dst, size_t* dst_len, 
					const unsigned char* src, size_t src_len)
{
	CLzmaEncProps props;
	LzmaEncProps_Init(&props);
	props.level = -1;
	props.dictSize = 0;
	props.lc = -1;
	props.lp = -1;
	props.pb = -1;
	props.fb = -1;
	props.numThreads = -1;

	size_t buf_sz = src_len * 2;
	uint8_t* buf = new uint8_t[buf_sz];

	memcpy(buf + PropHeaderSize, &src_len, 4);

	size_t out_sz = buf_sz;
	size_t prop_sz = PropHeaderSize;
	int result = LzmaEncode(buf + HeaderSize, &out_sz, src, src_len, &props, buf + 4, &prop_sz, 
		0, NULL, &g_Alloc, &g_Alloc);
	assert(result == SZ_OK);

	*dst_len = out_sz + HeaderSize;

	*dst = new uint8_t[*dst_len];
	memcpy(*dst, buf, *dst_len);

	(*dst)[0] = (src_len >> 24) & 0xff;
	(*dst)[1] = (src_len >> 16) & 0xff;
	(*dst)[2] = (src_len >> 8) & 0xff;
	(*dst)[3] = (src_len) & 0xff;

	delete[] buf;
}

int Lzma::Uncompress(unsigned char* dst, size_t* dst_len, 
					 const unsigned char* src, size_t* src_len, 
					 const unsigned char* props, size_t props_sz)
{
	ELzmaStatus status;
	return LzmaDecode(dst, dst_len, src, src_len, props, (unsigned)props_sz, LZMA_FINISH_ANY, &status, &g_Alloc);
}

}