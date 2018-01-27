#include "ee0/Subject.h"
#include "ee0/Observer.h"

namespace ee0
{

Subject::Subject(MessageID id)
	: m_id(id) 
{
}

void Subject::RegisterObserver(Observer* o) 
{
	m_observers.insert(o);
}

void Subject::UnregisterObserver(Observer* o)
{
	m_observers.erase(o);
}

void Subject::NotifyObservers(const VariantSet& variants) 
{
	for (auto& o : m_observers) {
		o->OnNotify(m_id, variants);
	}
}

}