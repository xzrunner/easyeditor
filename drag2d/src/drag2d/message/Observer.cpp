#include "Observer.h"
#include "Subject.h"

namespace d2d
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
	for (int i = 0; i < m_subjects.size(); ++i) {
		m_subjects[i]->Register(this);
	}
}

void Observer::UnRegistSubjects()
{
	for (int i = 0; i < m_subjects.size(); ++i) {
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

}