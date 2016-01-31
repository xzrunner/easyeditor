#pragma once


#include <json/json.h>

namespace escale9
{
	class StagePanel;
	class ToolbarPanel;

	class FileIO
	{
	public:
		static void load(const char* filename, ee::LibraryPanel* library, 
			ee::MultiSpritesImpl* stage, ToolbarPanel* toolbar);
		static void store(const char* filename, StagePanel* stage, 
			ToolbarPanel* toolbar);

	private:
		static ee::Sprite* load(const Json::Value& value, const wxString& dir);
		static Json::Value store(ee::Sprite* sprite, const wxString& dir);
		static Json::Value StoreNew(ee::Sprite* sprite, const wxString& dir);

	}; // FileIO
}

