
#include "Context.h"

#include <stdio.h>

using namespace eparticle;

Context* Context::m_instance = NULL;

Context::Context() 
{
	stage = NULL;
	toolbar = NULL;
}

Context* Context::Instance()
{
	if (!m_instance)
	{
		m_instance = new Context();
	}
	return m_instance;
}
