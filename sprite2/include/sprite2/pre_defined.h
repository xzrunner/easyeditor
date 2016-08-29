#ifndef _SPRITE2_PRE_DEFINED_H_
#define _SPRITE2_PRE_DEFINED_H_

#include "s2_config.h"

namespace s2
{

#ifdef S2_VIRTUAL_INHERITANCE
#error
	#define VIRTUAL_INHERITANCE virtual
	#define VI_DOWNCASTING dynamic_cast
	#define VI_DUMMY_FUNC \
		private: \
			virtual void __DUMMY__() {}
#pragma warning(disable:4250)
#else
	#define VIRTUAL_INHERITANCE
	#define VI_DOWNCASTING static_cast
	#define VI_DUMMY_FUNC
#endif // S2_VIRTUAL_INHERITANCE

#ifdef S2_VIRTUAL_INHERITANCE
	#define VI_CLONE(type, src) \
		dynamic_cast<type*>(static_cast<cu::Cloneable*>(src)->Clone());
#else
	#define VI_CLONE(type, src) \
		src->Clone();
#endif // S2_VIRTUAL_INHERITANCE

}

#endif // _SPRITE2_PRE_DEFINED_H_