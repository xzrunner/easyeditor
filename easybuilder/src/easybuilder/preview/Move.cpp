
#include "Move.h"

using namespace ebuilder::preview;

Move::Move(ee::ISprite* sprite, const ee::Vector& start, 
		   const ee::Vector& end, float speed)
	: m_sprite(sprite)
	, m_start(start)
	, m_end(end)
	, m_speed(speed)
	, m_startTime(0)
{
	AnimationControl::Instance()->insertObserver(this);
}

void Move::trigger()
{
	m_startTime = wxGetLocalTimeMillis();
}

void Move::onTick()
{
	if (m_startTime == 0) return;

	wxMilliClock_t curr = wxGetLocalTimeMillis();
	double during = (curr - m_startTime).ToDouble() * 0.001;

	float tot = ee::Math::getDistance(m_start, m_end) / m_speed;
	if (tot < during)
	{
		m_sprite->setTransform(m_end, m_sprite->getAngle());
		m_startTime = 0;
	}
	else
	{
		m_sprite->setTransform(m_start+(m_end-m_start)*(during/tot), m_sprite->getAngle());
	}
}
