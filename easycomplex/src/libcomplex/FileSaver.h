#pragma once

#include <string>

namespace ecomplex
{

class Symbol;

class FileSaver
{
public:
	static void Store(const std::string& filepath, const Symbol& sym,
		const std::string& dir);

private:
	

}; // FileSaver

}