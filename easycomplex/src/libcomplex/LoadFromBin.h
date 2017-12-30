#pragma once

#include <string>

namespace ecomplex
{

class Symbol;

class LoadFromBin
{
public:
	static void Load(const std::string& filepath, Symbol& complex);

}; // LoadFromBin

}