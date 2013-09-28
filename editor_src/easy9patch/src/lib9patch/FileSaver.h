#pragma once

#include <drag2d.h>

namespace lib9patch
{
	class FileSaver
	{
	public:
		static void store(const char* filepath, const d2d::Patch9Symbol& symbol);

//		static void store(const std::string& filepath, const d2d::Patch9Symbol& symbol);

	private:
//		static Json::Value store(d2d::ISprite* sprite, const std::string& dlg);

	}; // FileSaver
}

