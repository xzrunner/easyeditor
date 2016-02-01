#include "Context.h"

#include <stddef.h>

namespace escale9
{

Context* Context::m_instance = NULL;

Context::Context() 
{
	property = NULL;
	library = NULL;
	stage = NULL;
	toolbar = NULL;

	isComposeOP = true;

	resizeCMPT = NULL;

	width = 600;
	height = 400;
}

Context* Context::Instance()
{
	if (!m_instance)
	{
		m_instance = new Context();
	}
	return m_instance;
}

} // escale9