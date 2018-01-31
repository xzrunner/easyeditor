#include "ee0/Subject.h"
#include "ee0/Observer.h"
#include "ee0/VariantSet.h"

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
	Observer* skip = nullptr;
	auto var = variants.GetVariant("skip_observer");
	if (var.m_type == VT_PVOID) {
		skip = static_cast<Observer*>(var.m_val.pv);
	}

	for (auto& o : m_observers) 
	{
		if (skip && skip == o) {
			continue;
		}
		o->OnNotify(m_id, variants);
	}
}

}