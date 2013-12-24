#pragma once

#include <wx/wx.h>

namespace d2d
{
	class ISymbol;

	class SymbolFactory
	{
	public:
		static ISymbol* create(const wxString& filepath);

	}; // SymbolFactory
}

