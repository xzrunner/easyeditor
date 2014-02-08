#pragma once

#include <drag2d.h>

#include "TexturesMgr.h"

namespace coceditor
{
	class LibraryPanel;
	class StagePanel;
	class ToolbarPanel;
	class TexturesMgr;

	class Context
	{
	public:
		d2d::PropertySettingPanel* property;
		LibraryPanel* library;
		StagePanel* stage;
		ToolbarPanel* toolbar;

		int id;

		TexturesMgr texMgr;
//		std::vector<complex::Symbol*> symbols;
		std::vector<d2d::ISymbol*> symbols;
		wxString dlgPath;

		float scale;

	public:
		static Context* Instance();

	private:
		Context();

	private:
		static Context* m_instance;

	}; // Context
}

