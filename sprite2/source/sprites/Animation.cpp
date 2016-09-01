#include "Animation.h"

namespace s2
{

SINGLETON_DEFINITION(Animation)

Animation::Animation()
	: m_time(0)
{
}

void Animation::Update(float dt)
{
	m_time += dt;
}

}