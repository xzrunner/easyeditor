#ifndef FORMATION_LOVE2D_CODE_H
#define FORMATION_LOVE2D_CODE_H

#include <easybuilder.h>

namespace formation
{
	class Love2dCode
	{
	public:
		Love2dCode(ebuilder::CodeGenerator& gen);

		void resolve();

	private:
		ebuilder::CodeGenerator& m_gen;

	}; // Love2dCode
}

#endif // FORMATION_LOVE2D_CODE_H