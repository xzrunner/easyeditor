#include "Lzma.h"

#include <LzmaEnc.h>
#include <LzmaDec.h>
#include <Alloc.h>

namespace epbin
{

static void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
static void SzFree(void *p, void *address) { p = p; MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };

int Lzma::Compress(Block& block, const unsigned char* buf, size_t len)
{
	block = new uint8_t[len + sizeof(struct Block)];

	size_t dst_len = len + sizeof(block->data);
	size_t prop_len = LZMA_PROPS_SIZE;
	int r = Compress(block->data, &dst_len, buf, len, block->prop, &prop_len);
	if (r != SZ_OK) {
		delete[] block;
		return 0;
	}

	block->size[0] = (len >> 24) & 0xff;
	block->size[1] = (len >> 16) & 0xff;
	block->size[2] = (len >> 8) & 0xff;
	block->size[3] = (len) & 0xff;

	delete[] block;
	return 1;
}

int Lzma::Compress(unsigned char* dst, size_t* dst_len, 
				   const unsigned char* src, size_t src_len, 
				   unsigned char* out_props, size_t* out_props_sz)
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

	return LzmaEncode(dst, dst_len, src, src_len, &props, out_props, out_props_sz, 0,
		NULL, &g_Alloc, &g_Alloc);
}

int Lzma::Uncompress(unsigned char* dst, size_t* dst_len, 
					 const unsigned char* src, size_t* src_len, 
					 const unsigned char* props, size_t props_sz)
{
	ELzmaStatus status;
	return LzmaDecode(dst, dst_len, src, src_len, props, (unsigned)props_sz, LZMA_FINISH_ANY, &status, &g_Alloc);
}

}