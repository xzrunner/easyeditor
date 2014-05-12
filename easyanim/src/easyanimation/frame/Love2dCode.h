#pragma once

#include <easybuilder.h>

namespace eanim
{
	class Love2dCode
	{
	public:
		Love2dCode(ebuilder::CodeGenerator& gen);

		void resolve();

	private:
		int query(const std::string& filepath) const;

	private:
		ebuilder::CodeGenerator& m_gen;

		d2d::TexPackerAdapter m_packerAdapter;

	}; // Love2dCode
}

