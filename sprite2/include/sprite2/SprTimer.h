#ifndef _SPRITE2_SPR_TIMER_H_
#define _SPRITE2_SPR_TIMER_H_

#include <CU_Singleton.h>

namespace s2
{

class SprTimer
{
public:
	void Update(float dt);

	SINGLETON_DECLARATION(SprTimer)

}; // SprTimer

}

#endif // _SPRITE2_SPR_TIMER_H_