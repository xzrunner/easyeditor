#pragma once
#include <easybuilder.h>

namespace eparticle
{
namespace coco
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
}