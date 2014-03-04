#ifndef _EASYPARTICLE3D_CODE_H_
#define _EASYPARTICLE3D_CODE_H_

#include <easybuilder.h>

namespace eparticle3d
{

class Code
{
public:
	Code(ebuilder::CodeGenerator& gen);

	void resolve();

private:
	ebuilder::CodeGenerator& m_gen;

}; // Code

}

#endif // _EASYPARTICLE3D_CODE_H_