#ifndef _EPBIN_LZMA_H_
#define _EPBIN_LZMA_H_

#include "typedef.h"

namespace epbin
{

class Lzma
{
public:
	static void Compress(unsigned char** dst, size_t* dst_len, 
		const unsigned char* src, size_t src_len);

	static int Uncompress(unsigned char* dst, size_t* dst_len, 
		const unsigned char* src, size_t* src_len,
		const unsigned char* props, size_t props_sz);

}; // Lzma

}

#endif // _EPBIN_LZMA_H_