#pragma once


#include <json/json.h>

namespace coceditor
{
	class FileIO
	{
	public:
		static void load(const char* filename);
		static void store(const char* filename);

	private:
		static ee::ISprite* load(const Json::Value& value, const std::string& dlg);
		static Json::Value store(ee::ISprite* sprite, const std::string& dlg);

	}; // FileIO
}

