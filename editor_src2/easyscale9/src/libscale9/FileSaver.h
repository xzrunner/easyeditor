#pragma once

#include <drag2d.h>

namespace escale9
{
	class Symbol;

	class FileSaver
	{
	public:
		static void store(const char* filepath, const Symbol& symbol);

//		static void store(const std::string& filepath, const Symbol& symbol);

	private:
//		static Json::Value store(d2d::ISprite* sprite, const std::string& dlg);

	}; // FileSaver
}

