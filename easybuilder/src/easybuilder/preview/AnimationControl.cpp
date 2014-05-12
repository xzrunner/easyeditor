
#include "AnimationControl.h"
#include "Canvas.h"

using namespace ebuilder::preview;

AnimationControl* AnimationControl::m_instance = NULL;

BEGIN_EVENT_TABLE(AnimationControl, wxWindow)
	EVT_TIMER(TIMER_ID, AnimationControl::onTimer)
END_EVENT_TABLE()

AnimationControl::AnimationControl()
	: m_canvas(NULL)
	, m_timer(this, TIMER_ID)
{
}

AnimationControl* AnimationControl::Instance()
{
	if (!m_instance)
	{
		m_instance = new AnimationControl();
	}
	return m_instance;
}

void AnimationControl::insertObserver(ITimeObserver* observer)
{
	m_observers.insert(observer);
}

void AnimationControl::removeObserver(ITimeObserver* observer)
{
	m_observers.erase(observer);
}

void AnimationControl::clear()
{
	m_observers.clear();
}

void AnimationControl::onTimer(wxTimerEvent& event)
{
	notifyObservers();
	if (m_canvas) m_canvas->Refresh();
}

void AnimationControl::notifyObservers()
{
	std::set<ITimeObserver*>::iterator itr = m_observers.begin();
	for ( ; itr != m_observers.end(); ++itr)
		(*itr)->onTick();
}
