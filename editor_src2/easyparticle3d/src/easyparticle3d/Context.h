#ifndef _EASYPARTICLE3D_CONTEXT_H_
#define _EASYPARTICLE3D_CONTEXT_H_

#include <drag2d.h>

namespace eparticle3d
{

class LibraryPanel;
class StagePanel;
class ToolbarPanel;

class Context
{
public:
	LibraryPanel* library;
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

#endif // _EASYPARTICLE3D_CONTEXT_H_
