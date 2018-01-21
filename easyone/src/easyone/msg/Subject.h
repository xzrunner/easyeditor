#pragma once

#include "msg/Observer.h"

#include <memory>
#include <set>

namespace eone
{

class VariantSet;

class Subject
{
public:
	Subject(int id) : m_id(id) {}
	virtual ~Subject() {}

	void RegisterObserver(const std::shared_ptr<Observer>& o) {
		m_observers.insert(o);
	}
	void UnregisterObserver(const std::shared_ptr<Observer>& o) {
		m_observers.erase(o);
	}

	void NotifyObservers(const VariantSet& variants) {
		for (auto& o : m_observers) {
			o->OnNotify(m_id, variants);
		}
	}

private:
	int m_id;

	std::set<std::shared_ptr<Observer>> m_observers;

}; // Subject

}