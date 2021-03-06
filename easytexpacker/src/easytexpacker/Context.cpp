#include "Context.h"

#include <stdio.h>

namespace etexpacker
{

Context* Context::m_instance = NULL;

Context::Context() 
{
	property = NULL;
	library = NULL;
	stage = NULL;
	toolbar = NULL;

	width = height = 0;
	padding = 0;
	extrude = 0;
	scale = 1.0f;

	auto_arrange = true;
	premultiplied_alpha = false;
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
