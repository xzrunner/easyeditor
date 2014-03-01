
#include "Actor.h"

#include "behaviors/Behavior.h"

using namespace ebuilder;

Actor::Actor(const Actor& actor)
	: d2d::ImageSprite(actor)
{
	m_isBg = actor.m_isBg;
}

Actor::Actor(d2d::ImageSymbol* symbol)
	: d2d::ImageSprite(symbol)
	, m_isBg(false)
{
	static int count = 0;
	name = wxT("actor") + wxString::FromDouble(count++);
}

Actor::~Actor()
{
	clearBehaviors();	
}

Actor* Actor::clone()
{
	return new Actor(*this);
}

void Actor::clearBehaviors()
{
	for_each(m_behaviors.begin(), m_behaviors.end(), DeletePointerFunctor<Behavior>());
	m_behaviors.clear();
}
