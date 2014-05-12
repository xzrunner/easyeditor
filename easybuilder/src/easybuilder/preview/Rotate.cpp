
#include "Rotate.h"

using namespace ebuilder::preview;

Rotate::Rotate(d2d::ISprite* sprite, float start, 
			   float end, float speed)
	: m_sprite(sprite)
	, m_start(start)
	, m_end(end)
	, m_speed(speed)
	, m_startTime(0)
{
	AnimationControl::Instance()->insertObserver(this);
}

void Rotate::trigger()
{
	m_startTime = wxGetLocalTimeMillis();
}

void Rotate::onTick()
{
	if (m_startTime == 0) return;

	wxMilliClock_t curr = wxGetLocalTimeMillis();
	double during = (curr - m_startTime).ToDouble() * 0.001;

	float tot = fabs(m_end - m_start) / m_speed;
	if (tot < during)
	{
		m_sprite->setTransform(m_sprite->getPosition(), m_end);
		m_startTime = 0;
	}
	else
	{
		m_sprite->setTransform(m_sprite->getPosition(), m_start+(m_end-m_start)*(during/tot));
	}
}
