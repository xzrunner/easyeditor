#ifndef _DRAG2D_SUBJECT_H_
#define _DRAG2D_SUBJECT_H_

#include <set>

namespace d2d
{

class Observer;

class Subject
{
public:
	Subject(int id) : m_id(id) {}
	virtual ~Subject() {}

	void Register(Observer* observer);
	void UnRegister(Observer* observer);
	
	void Notify(void* ud, Observer* except = NULL);

private:
	int m_id;

	std::set<Observer*> m_observers;

}; // Subject

}

#endif // _DRAG2D_SUBJECT_H_