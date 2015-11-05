#include "UIStagePage.h"

namespace eui
{

void UIStagePage::RegistSubjects(d2d::Observer* observer)
{
	for (int i = 0, n = m_subjects.size(); i < n; ++i) {
		m_subjects[i]->Register(observer);
	}
}

void UIStagePage::UnRegistSubjects(d2d::Observer* observer)
{
	for (int i = 0, n = m_subjects.size(); i < n; ++i) {
		m_subjects[i]->UnRegister(observer);
	}
}

}