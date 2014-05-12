#ifndef _EASYPARTICLE3D_CODE_H_
#define _EASYPARTICLE3D_CODE_H_

#include <easybuilder.h>

namespace eparticle3d
{

class ToolbarPanel;

class Code
{
public:
	Code(ebuilder::CodeGenerator& gen, ToolbarPanel* toolbar);

	void resolve();

private:
	ebuilder::CodeGenerator& m_gen;

	ToolbarPanel* m_toolbar;

}; // Code

}

#endif // _EASYPARTICLE3D_CODE_H_