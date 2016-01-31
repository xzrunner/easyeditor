#include "Observer.h"
#include "Subject.h"

namespace ee
{

Observer::Observer()
	: m_observe_enable(true)
{
}

Observer::~Observer()
{
	UnRegistSubjects();
}

void Observer::RegistSubject(Subject* sbj)
{
	sbj->Register(this);
	m_subjects.push_back(sbj);
}

void Observer::RegistSubjects()
{
	for (int i = 0, n = m_subjects.size(); i < n; ++i) {
		m_subjects[i]->Register(this);
	}
}

void Observer::UnRegistSubjects()
{
	for (int i = 0, n = m_subjects.size(); i < n; ++i) {
		m_subjects[i]->UnRegister(this);
	}	
}

void Observer::Notify(int sj_id, void* ud)
{
	if (!m_observe_enable) {
		return;
	}

	OnNotify(sj_id, ud);
}

void Observer::EnableObserve(bool enable) 
{ 
	m_observe_enable = enable; 
}

}