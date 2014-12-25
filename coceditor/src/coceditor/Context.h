#pragma once

#include <drag2d.h>

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
		d2d::PropertySettingPanel* property;
		LibraryPanel* library;
		StagePanel* stage;
		ToolbarPanel* toolbar;

		int id;

		TextureMgr texMgr;
		std::vector<const d2d::ISymbol*> symbols;
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

