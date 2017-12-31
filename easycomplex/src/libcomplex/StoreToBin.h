#pragma once

#include <string>

namespace ecomplex
{

class Symbol;

class StoreToBin
{
public:
	static void Store(const std::string& filepath, 
		const Symbol& sym, const std::string& dir);

}; // StoreToBin

}