#pragma once

#include <wx/wx.h>
#include <map>

namespace d2d
{
	class ISymbol;
	class IVisitor;

	class SymbolMgr
	{
	public:
		static SymbolMgr* Instance();

		ISymbol* getSymbol(const wxString& filepath);

		void clear();

		void traverse(IVisitor& visitor) const;

	private:
		SymbolMgr();
		~SymbolMgr();

	private:
		static SymbolMgr* m_instance;

		std::map<wxString, ISymbol*> m_symbols;

	}; // SymbolMgr
}

