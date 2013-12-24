#pragma once

#include <drag2d.h>
#include <json/json.h>

namespace e9patch
{
	class FileIO
	{
	public:
		static void load(const char* filename);
		static void store(const char* filename);

	private:
		static d2d::ISprite* load(const Json::Value& value, const std::string& dlg);
		static Json::Value store(d2d::ISprite* sprite, const std::string& dlg);

	}; // FileIO
}

