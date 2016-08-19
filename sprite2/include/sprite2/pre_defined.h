#ifndef _SPRITE2_PRE_DEFINED_H_
#define _SPRITE2_PRE_DEFINED_H_

namespace s2
{

#ifdef EASY_EDITOR
	#define S2_VIRTUAL_INHERITANCE
#endif // EASY_EDITOR

#ifdef S2_VIRTUAL_INHERITANCE
	#define VIRTUAL_INHERITANCE virtual
	#define VI_DOWNCASTING dynamic_cast
	#define VI_DUMMY_FUNC \
		private: \
			virtual void __DUMMY__() {}
#pragma warning(disable:4250)
#else
#error
	#define VIRTUAL_INHERITANCE
	#define VI_DOWNCASTING static_cast
	#define VI_DUMMY_FUNC
#endif // S2_VIRTUAL_INHERITANCE

}

#endif // _SPRITE2_PRE_DEFINED_H_