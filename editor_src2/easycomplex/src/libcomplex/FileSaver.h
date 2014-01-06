#pragma once

#include <drag2d.h>

namespace libcomplex
{
	class FileSaver
	{
	public:
		static void store(const char* filepath, const d2d::ComplexSymbol* symbol);
		static void storeWithHistory(const char* filepath, const d2d::ComplexSymbol* symbol,
			d2d::EditPanel* editpanel);

	private:
		static void centerSymbol(d2d::ComplexSymbol* symbol);
		static Json::Value store(d2d::ISprite* sprite, const wxString& dir);

	}; // FileSaver
}

