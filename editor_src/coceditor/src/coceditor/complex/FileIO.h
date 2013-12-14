#pragma once

#include <drag2d.h>

namespace coceditor
{
namespace complex
{
	class FileIO
	{
	public:
		static void store(const char* filepath, const d2d::ComplexSymbol* complexSymbol);

	private:
		static Json::Value store(d2d::ISprite* sprite);

	}; // FileIO
}
}

