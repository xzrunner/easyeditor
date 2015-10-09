#ifndef _LIBRESPACKER_LZMA_H_
#define _LIBRESPACKER_LZMA_H_

#include <stdint.h>

namespace librespacker
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

#endif // _LIBRESPACKER_LZMA_H_