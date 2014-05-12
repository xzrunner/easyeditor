#include "Context.h"

namespace eanim
{

Context* Context::m_instance = NULL;

Context::Context() 
{
	library = NULL;
	timeline = NULL;
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

} // eanim