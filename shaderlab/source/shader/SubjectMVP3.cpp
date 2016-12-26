#include "SubjectMVP3.h"
#include "ObserverMVP.h"

#include <string.h>

namespace sl
{

SubjectMVP3* SubjectMVP3::m_instance = NULL;

SubjectMVP3* SubjectMVP3::Instance()
{
	if (!m_instance) {
		m_instance = new SubjectMVP3;
	}
	return m_instance;
}

SubjectMVP3::SubjectMVP3()
{
	m_modelview.Identity();
	m_projection.Identity();
}

void SubjectMVP3::NotifyModelview(const sm::mat4& mat)
{
	m_modelview = mat;
	std::set<ObserverMVP*>::iterator itr = m_observers.begin();
	for ( ; itr != m_observers.end(); ++itr) {
		(*itr)->SetModelview(&m_modelview);
	}
}

void SubjectMVP3::NotifyProjection(const sm::mat4& mat)
{
	m_projection = mat;
	std::set<ObserverMVP*>::iterator itr = m_observers.begin();
	for ( ; itr != m_observers.end(); ++itr) {
		(*itr)->SetProjection(&m_projection);
	}
}

}