#ifndef _SPRITE2_ANIMATION_H_
#define _SPRITE2_ANIMATION_H_

#include <CU_Singleton.h>

namespace s2
{

class Animation
{
public:
	void Update(float dt);

	float GetTime() const { return m_time; }

private:
	float m_time;

	SINGLETON_DECLARATION(Animation)

}; // Animation

}

#endif // _SPRITE2_ANIMATION_H_