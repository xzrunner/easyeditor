#include "SubjectMVP2.h"
#include "ObserverMVP.h"

#include <stddef.h>

namespace sl
{

SubjectMVP2* SubjectMVP2::m_instance = NULL;

SubjectMVP2* SubjectMVP2::Instance()
{
	if (!m_instance) {
		m_instance = new SubjectMVP2;
	}
	return m_instance;
}

SubjectMVP2::SubjectMVP2()
{
	m_modelview.Identity();
	m_projection.Identity();
}

void SubjectMVP2::NotifyModelview(float x, float y, float sx, float sy)
{
	m_modelview = sm::mat4::Scaled(sx, sy, 1);
	m_modelview.Translate(x * sx, y * sy, 0);
	std::set<ObserverMVP*>::iterator itr = m_observers.begin();
	for ( ; itr != m_observers.end(); ++itr) {
		(*itr)->SetModelview(&m_modelview);
	}
}

void SubjectMVP2::NotifyProjection(int width, int height)
{
	float hw = width * 0.5f;
	float hh = height * 0.5f;
	m_projection = sm::mat4::Orthographic(-hw, hw, -hh, hh, 1, -1);
	std::set<ObserverMVP*>::iterator itr = m_observers.begin();
	for ( ; itr != m_observers.end(); ++itr) {
		(*itr)->SetProjection(&m_projection);
	}
}

}