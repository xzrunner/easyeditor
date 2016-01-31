#ifndef _EASYEDITOR_SUBJECT_H_
#define _EASYEDITOR_SUBJECT_H_

#include <set>

namespace ee
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

#define SUBJECT_DECLARATION(name) \
	public: \
		static name* Instance(); \
	private: \
		(name)(int id); \
	private: \
		static name* m_instance;

#define SUBJECT_DEFINITION(name, s_id) \
	name* name::m_instance = NULL; \
	name::name(int id) : Subject(id) {} \
	name* name::Instance() \
	{ \
		if (!m_instance) { \
			m_instance = new (name)(s_id); \
		} \
		return m_instance; \
	}

}

#endif // _EASYEDITOR_SUBJECT_H_