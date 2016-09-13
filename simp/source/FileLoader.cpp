#include "FileLoader.h"
#include "ImportStream.h"
#include "simp_define.h"

#include <fault.h>
#include <fs_file.h>

#include <Alloc.h>
#include <LzmaDec.h>

#include <stdint.h>

namespace simp
{

static char* CACHE_BUF = NULL;
static int CACHE_SZ = 0;

#define PACKAGE_SIZE 512
#define UNUSED(x)	((void)(x))

#define LZMA_PROPS_SIZE 5

static void *SzAlloc(void *p, size_t size) { UNUSED(p); return MyAlloc(size); }
static void SzFree(void *p, void *address) { UNUSED(p); MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };

static inline int
_lzma_uncompress(unsigned char *dest, size_t  *destLen, const unsigned char *src, size_t  *srcLen, 
				 const unsigned char *props, size_t propsSize)
{
	ELzmaStatus status;
	return LzmaDecode(dest, destLen, src, srcLen, props, (unsigned)propsSize, LZMA_FINISH_ANY, &status, &g_Alloc);
}

struct block {
	uint8_t size[4];
	uint8_t prop[LZMA_PROPS_SIZE];
	uint8_t data[119];
};

FileLoader::FileLoader(const std::string& filepath)
	: m_filepath(filepath)
{
}

void FileLoader::Load()
{
	fs_file* file = fs_open(m_filepath.c_str(), "rb");
	if (!file) {
		fault("open file fail: %s\n", m_filepath.c_str());
		return;
	}
	
	int32_t sz = 0;
	fs_read(file, &sz, sizeof(sz));
	if (sz < 0)
	{
		sz = -sz;

		CachePrepare(sz);
		char* buf = CACHE_BUF;
		if (fs_read(file, (char*)buf, sz) != sz) {
			fault("Invalid uncompress data source\n");
		}

		ImportStream is(buf, sz);
		OnLoad(is);
	}
	else
	{
		uint8_t ori_sz_arr[4];
		fs_read(file, ori_sz_arr, sizeof(ori_sz_arr));
		fs_seek_from_cur(file, -(int)sizeof(ori_sz_arr));
		size_t ori_sz = ori_sz_arr[0] << 24 | ori_sz_arr[1] << 16 | ori_sz_arr[2] << 8 | ori_sz_arr[3];
		size_t need_sz = sz + 7 + ori_sz;

		CachePrepare(need_sz);
		char* buf = CACHE_BUF;

		struct block* block = (struct block*)buf;
		if (sz <= 4 + LZMA_PROPS_SIZE || fs_read(file, block, sz) != sz) {
			fault("Invalid compress data source\n");
		}

		unsigned char* buffer = (unsigned char*)(buf + ALIGN_4BYTE(sz));
		size_t compressed_sz = sz - sizeof(block->size) - LZMA_PROPS_SIZE;

		int r = _lzma_uncompress(buffer, &ori_sz, block->data, &compressed_sz, block->prop, LZMA_PROPS_SIZE);
		if (r != SZ_OK) {
			fault("Uncompress error %d\n",r);
		}

		ImportStream is((char*)buffer, ori_sz);
		OnLoad(is);
	}

	fs_close(file);
}

void FileLoader::CachePrepare(int sz)
{
	if (!CACHE_BUF) {
		CACHE_BUF = new char[sz];
		if (!CACHE_BUF) {
			fault("FileLoader::CachePrepare fail\n");
		}
		CACHE_SZ = sz;
	} else if (CACHE_SZ < sz) {
		delete[] CACHE_BUF;
		CACHE_BUF = new char[sz];
		if (!CACHE_BUF) {
			fault("FileLoader::CachePrepare fail\n");
		}
		CACHE_SZ = sz;
	}
}

}