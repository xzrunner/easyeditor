
#include "Context.h"

#include "StagePanel.h"
#include "ToolbarPanel.h"

using namespace eshape;

Context* Context::m_instance = NULL;

Context::Context() 
{
	property = NULL;
	library = NULL;
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

void Context::changeCurrItem(LibraryItem* item)
{
	stage->changeCurrItem(item);
	toolbar->changeCurrItem(item);
}