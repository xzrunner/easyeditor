#ifndef _SPRITE2_PRE_DEFINED_H_
#define _SPRITE2_PRE_DEFINED_H_

namespace s2
{

#ifdef S2_VIRTUAL_INHERITANCE
	#define VIRTUAL_INHERITANCE virtual
	#define VI_DOWNCASTING dynamic_cast
#else
	#define VIRTUAL_INHERITANCE
	#define VI_DOWNCASTING static_cast
#endif // S2_VIRTUAL_INHERITANCE

}

#endif // _SPRITE2_PRE_DEFINED_H_