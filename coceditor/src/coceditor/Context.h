#pragma once



#include "TextureMgr.h"

namespace libcoco
{
	class LibraryPanel;
	class StagePanel;
	class ToolbarPanel;
	class TextureMgr;

	class Context
	{
	public:
		ee::PropertySettingPanel* property;
		LibraryPanel* library;
		StagePanel* stage;
		ToolbarPanel* toolbar;

		int id;

		TextureMgr texMgr;
		std::vector<const ee::ISymbol*> symbols;
		wxString dlgPath;

		std::set<std::string> ignore_list;

	public:
		static Context* Instance();

	private:
		Context();

	private:
		static Context* m_instance;

	}; // Context
}

