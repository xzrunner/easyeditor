#ifndef _SHADERLAB_SUBJECT_MVP_H_
#define _SHADERLAB_SUBJECT_MVP_H_

#include <SM_Matrix.h>

#include <set>

namespace sl
{

class ObserverMVP;

class SubjectMVP
{
public:
	void Register(ObserverMVP* observer) { m_observers.insert(observer); }
	void UnRegister(ObserverMVP* observer) { m_observers.erase(observer); }

	void Clear() { 
		m_observers.clear();
		m_modelview.Identity();
		m_projection.Identity();
	}

protected:
	std::set<ObserverMVP*> m_observers;

	sm::mat4 m_modelview, m_projection;

};	// SubjectMVP

}

#endif // _SHADERLAB_SUBJECT_MVP_H_
