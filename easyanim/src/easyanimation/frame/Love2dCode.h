#pragma once

#include <easybuilder.h>

namespace eanim
{
	class Controller;

	class Love2dCode
	{
	public:
		Love2dCode(ebuilder::CodeGenerator& gen,
			Controller* ctrl);

		void resolve();

	private:
		int query(const std::string& filepath) const;

	private:
		ebuilder::CodeGenerator& m_gen;

		Controller* m_ctrl;

		d2d::TexPackerAdapter m_packerAdapter;

	}; // Love2dCode
}

