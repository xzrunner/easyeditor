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

		ISymbol* fetchSymbol(const wxString& filepath);

		void clear();

		void traverse(IVisitor& visitor) const;

		size_t size() const {
			return m_symbols.size();
		}

	protected:
		void remove(const ISymbol* symbol);

	private:
		SymbolMgr();
		~SymbolMgr();

	private:
		static SymbolMgr* m_instance;

		std::map<wxString, ISymbol*> m_symbols;

		friend class ISymbol;

	}; // SymbolMgr
}

