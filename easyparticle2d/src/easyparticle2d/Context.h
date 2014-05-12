#ifndef _EASYPARTICLE2D_CONTEXT_H_
#define _EASYPARTICLE2D_CONTEXT_H_

#include <drag2d.h>

namespace eparticle2d
{

class StagePanel;
class ToolbarPanel;

class Context
{
public:
	StagePanel* stage;
	ToolbarPanel* toolbar;

public:
	static Context* Instance();

private:
	Context();

private:
	static Context* m_instance;

}; // Context

}

#endif // _EASYPARTICLE2D_CONTEXT_H_