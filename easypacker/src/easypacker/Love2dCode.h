#ifndef EPACKER_LOVE2D_CODE_H
#define EPACKER_LOVE2D_CODE_H

#include <easybuilder.h>

namespace epacker
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

#endif // EPACKER_LOVE2D_CODE_H