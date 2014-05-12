#pragma once

#include <drag2d.h>

namespace escale9
{
	class Symbol;

	class FileSaver
	{
	public:
		static void store(const char* filepath, const Symbol& symbol);

	}; // FileSaver
}

