#include "Context.h"

//#include <stdio.h>

namespace libcoco
{

Context* Context::m_instance = NULL;

Context::Context() 
{
	property = NULL;
	library = NULL;
	stage = NULL;
	toolbar = NULL;

	id = 0;
}

Context* Context::Instance()
{
	if (!m_instance)
	{
		m_instance = new Context();
	}
	return m_instance;
}

} // coceditor