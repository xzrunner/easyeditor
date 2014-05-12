#pragma once

#include <drag2d.h>

namespace ecomplex
{
	class Symbol;

	class FileSaver
	{
	public:
		static void store(const char* filepath, const Symbol* symbol);
		static void storeWithHistory(const char* filepath, const Symbol* symbol,
			d2d::EditPanel* editpanel);

	private:
		static void centerSymbol(Symbol* symbol);
		static Json::Value store(d2d::ISprite* sprite, const wxString& dir);

	}; // FileSaver
}

