#include "Context.h"

#include <easymodeling.h>

#include <stddef.h>

namespace emodeling
{

Context* Context::m_instance = NULL;

Context::Context() 
{
	property = NULL;
	library = NULL;
	stage = NULL;
	toolbar = NULL;

	world = new World;
}

Context::~Context()
{
	delete world;
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