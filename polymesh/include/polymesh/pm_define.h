#ifndef _POLYMESH_DEFINE_H_
#define _POLYMESH_DEFINE_H_

namespace pm
{

static const int PTR_SIZE_DIFF		= (8 - sizeof(void *));
static const int SIZEOF_POINTER		= 8;

#ifndef ALIGN_4BYTE
	#define ALIGN_4BYTE(x) (((x) + 3) & ~3)
#endif // ALIGN_4BYTE

}

#endif // _POLYMESH_DEFINE_H_