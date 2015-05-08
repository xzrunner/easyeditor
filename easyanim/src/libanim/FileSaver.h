#pragma once

#include <drag2d.h>

#include "Symbol.h"

namespace eanim
{

class FileSaver
{
public:
	static void store(const std::string& filepath, const Symbol& symbol);

private:
	static void store(Json::Value& value, Symbol::Layer* layer, const wxString& dir);
	static void store(Json::Value& value, Symbol::Frame* frame, const wxString& dir);
	static void store(Json::Value& value, d2d::ISprite* sprite, const wxString& dir);

}; // FileSaver

}

