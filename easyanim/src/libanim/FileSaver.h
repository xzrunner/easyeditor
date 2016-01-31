#pragma once



#include "Symbol.h"

namespace libanim
{

class FileSaver
{
public:
	static void store(const std::string& filepath, const Symbol& symbol);

private:
	static void store(Json::Value& value, Symbol::Layer* layer, const wxString& dir);
	static void store(Json::Value& value, Symbol::Frame* frame, const wxString& dir);
	static void store(Json::Value& value, ee::Sprite* sprite, const wxString& dir);

}; // FileSaver

}

