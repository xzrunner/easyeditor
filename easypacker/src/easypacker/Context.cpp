#include "Context.h"

#include <stdio.h>

using namespace epacker;

Context* Context::m_instance = NULL;

Context::Context() 
{
	property = NULL;
	library = NULL;
	stage = NULL;
	toolbar = NULL;

	width = height = 0;
	padding = 0;
	scale = 1.0f;

	auto_arrange = true;
}

Context* Context::Instance()
{
	if (!m_instance)
	{
		m_instance = new Context();
	}
	return m_instance;
}
