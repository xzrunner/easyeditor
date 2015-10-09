#ifndef _EASYTEXPACKER_LOVE2D_CODE_H_
#define _EASYTEXPACKER_LOVE2D_CODE_H_

#include <easybuilder.h>

namespace etexpacker
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

#endif // _EASYTEXPACKER_LOVE2D_CODE_H_