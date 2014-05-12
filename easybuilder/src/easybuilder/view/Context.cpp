
#include "Context.h"

#include <stdio.h>

using namespace ebuilder;

Context* Context::m_instance = NULL;

Context::Context() 
{
	library = NULL;
	property = NULL;
	backstage = NULL;
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
