#include "SprTimer.h"
#include "Animation.h"
#include "Particle2d.h"
#include "Particle3d.h"
#include "Trail.h"

namespace s2
{

SINGLETON_DEFINITION(SprTimer)

SprTimer::SprTimer()
{
}

void SprTimer::Init()
{
	Animation::Instance();
	Particle2d::Instance();
	Particle3d::Instance();
	Trail::Instance();
}

void SprTimer::Update(float dt)
{
	Animation::Instance()->Update(dt);
	Particle2d::Instance()->Update(dt);
	Particle3d::Instance()->Update(dt);
	Trail::Instance()->Update(dt);
}

}