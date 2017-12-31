#pragma once

#include <rapidjson/document.h>

namespace js
{

class RapidJsonHelper
{
public:
	static void ReadFromFile(const char* filepath, rapidjson::Document& doc);

	static void WriteToFile(const char* filepath, const rapidjson::Document& doc);

}; // RapidJson

}