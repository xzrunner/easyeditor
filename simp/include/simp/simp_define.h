#ifndef _SIMP_DEFINES_H_
#define _SIMP_DEFINES_H_

namespace simp
{

static const int PTR_SIZE_DIFF		= (8 - sizeof(void *));
static const int SIZEOF_POINTER		= 8;

static const int ANCHOR_ID			= 0xffffffff;
static const int SCREEN_SCALE		= 16;

static const int PAGE_SIZE			= 4096;

#define ALIGN_4BYTE(x) (((x) + 3) & ~3)

}

#endif // _SIMP_DEFINES_H_