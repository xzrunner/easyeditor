#ifndef _EPBIN_LZMA_H_
#define _EPBIN_LZMA_H_

#include "typedef.h"

namespace epbin
{

#define LZMA_PROPS_SIZE 5

class Lzma
{
public:
	struct Block 
	{
		uint8_t size[4];
		uint8_t prop[LZMA_PROPS_SIZE];
		uint8_t data[119];
	};

public:
	static int Compress(Block& block, const unsigned char* buf, size_t len);

	static int Compress(unsigned char* dst, size_t* dst_len, 
		const unsigned char* src, size_t src_len,
		unsigned char* out_props, size_t* out_props_sz);
	static int Uncompress(unsigned char* dst, size_t* dst_len, 
		const unsigned char* src, size_t* src_len,
		const unsigned char* props, size_t props_sz);

}; // Lzma

}

#endif // _EPBIN_LZMA_H_