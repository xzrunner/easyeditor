#ifndef _EASYEDITOR_SINGLETON_H_
#define _EASYEDITOR_SINGLETON_H_

#include <stddef.h>

namespace ee
{

#define SINGLETON_DECLARATION(name) \
	public: \
		static name* Instance(); \
	private: \
		name(); \
		~name(); \
	private: \
		static name* m_instance;

#define SINGLETON_DEFINITION(name) \
	name* name::m_instance = NULL; \
	name* name::Instance() \
	{ \
		if (!m_instance) { \
			m_instance = new (name)(); \
		} \
		return m_instance; \
	}

}

#endif // _EASYEDITOR_SINGLETON_H_