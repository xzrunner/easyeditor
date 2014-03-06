#pragma once

#include <drag2d.h>

namespace libscale9
{
	class FileSaver
	{
	public:
		static void store(const char* filepath, const d2d::Scale9Symbol& symbol);

//		static void store(const std::string& filepath, const d2d::Scale9Symbol& symbol);

	private:
//		static Json::Value store(d2d::ISprite* sprite, const std::string& dlg);

	}; // FileSaver
}

