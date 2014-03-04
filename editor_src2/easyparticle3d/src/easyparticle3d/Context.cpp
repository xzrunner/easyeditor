#include "Context.h"

#include <stdio.h>

namespace eparticle3d
{

Context* Context::m_instance = NULL;

Context::Context() 
{
	stage = NULL;
	toolbar = NULL;
	library = NULL;
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
