#include "Context.h"

#include <stddef.h>

namespace edb
{

Context* Context::m_instance = NULL;

Context::Context() 
{
	stage = NULL;
}

Context* Context::Instance()
{
	if (!m_instance)
	{
		m_instance = new Context();
	}
	return m_instance;
}

}