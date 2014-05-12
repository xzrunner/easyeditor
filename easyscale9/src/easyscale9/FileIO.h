#pragma once

#include <drag2d.h>
#include <json/json.h>

namespace escale9
{
	class StagePanel;
	class ToolbarPanel;

	class FileIO
	{
	public:
		static void load(const char* filename, d2d::LibraryPanel* library, 
			d2d::MultiSpritesImpl* stage, ToolbarPanel* toolbar);
		static void store(const char* filename, StagePanel* stage, 
			ToolbarPanel* toolbar);

	private:
		static d2d::ISprite* load(const Json::Value& value, const wxString& dir);
		static Json::Value store(d2d::ISprite* sprite, const wxString& dir);

	}; // FileIO
}

