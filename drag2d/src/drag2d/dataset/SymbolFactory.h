#pragma once

#include <wx/wx.h>
#include <map>

namespace d2d
{
	class ISymbol;

	class SymbolFactory
	{
	public:
		static ISymbol* create(const wxString& filepath);

		typedef ISymbol* (*CreateCallback)();
		static void RegisterCreator(const std::string& type, CreateCallback cb);
		static void UnregisterCreator(const std::string& type);

	private:
		typedef std::map<std::string, CreateCallback> CallbackMap;
		static CallbackMap m_creators;

	}; // SymbolFactory
}

