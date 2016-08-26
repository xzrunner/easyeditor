#ifndef _SPRITE2_TRAIL_H_
#define _SPRITE2_TRAIL_H_

#include "RenderColor.h"

#include <CU_Singleton.h>
#include <SM_Matrix.h>

namespace s2
{

class TrailRenderParams
{
public:
	sm::mat4 mat;
	RenderColor ct;
}; // TrailRenderParams

class Trail
{
public:
	void Update(float dt);

	float GetTime() const { return m_time; }

private:
	void Init();

private:
	float m_time;

	SINGLETON_DECLARATION(Trail)

}; // Trail

}

#endif // _SPRITE2_TRAIL_H_