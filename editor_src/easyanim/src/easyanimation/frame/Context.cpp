#include "Context.h"

namespace eanim
{

Context* Context::m_instance = NULL;

Context::Context() 
{
	library = NULL;
	property = NULL;
	stage = NULL;
	toolbar = NULL;
	layersPanel = NULL;
	keysPanel = NULL;

	currLayer = -1;
	currFrame = -1;
	maxFrame = -1;

	fps = 30;
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