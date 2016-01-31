
#include "ListObserver.h"

#include "behaviors/Behavior.h"
#include "behaviors/BehaviorPanel.h"

using namespace ebuilder;

ListObserver::ListObserver(ee::LibraryList* list)
	: m_list(list)
{
}

void ListObserver::registerListener(Behavior* lsn)
{
	m_listeners.push_back(lsn);
}

void ListObserver::registerListener(BehaviorPanel* lsn)
{
	m_uiListeners.push_back(lsn);
}

void ListObserver::removeListener(Behavior* lsn)
{
	for (size_t i = 0, n = m_listeners.size(); i < n; ++i)
	{
		if (lsn == m_listeners[i])
		{
			m_listeners.erase(m_listeners.begin() + i);
			break;
		}
	}
}

void ListObserver::clearUIListeners()
{
	m_uiListeners.clear();
}

void ListObserver::updateListeners()
{
	for (size_t i = 0, n = m_listeners.size(); i < n; ++i)
		m_listeners[i]->update(*m_list);

	for (size_t i = 0, n = m_uiListeners.size(); i < n; ++i)
		m_uiListeners[i]->update(*m_list);
}
