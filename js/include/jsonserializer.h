#pragma once

#include <rapidjson/document.h>

namespace jsons
{

inline 
void read_from_file(const char* filepath, rapidjson::Document& doc);

inline 
void write_to_file(const char* filepath, const rapidjson::Document& doc);

}