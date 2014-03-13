#pragma once

#include <drag2d.h>

#include "Symbol.h"

namespace anim
{

class FileSaver
{
public:
	static void store(const std::string& filepath, const Symbol& symbol);

private:
	static Json::Value store(Symbol::Layer* layer, const wxString& dir);
	static Json::Value store(Symbol::Frame* frame, const wxString& dir);
	static Json::Value store(d2d::ISprite* sprite, const wxString& dir);

}; // FileSaver

}

