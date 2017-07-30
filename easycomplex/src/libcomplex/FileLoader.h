#ifndef _EASYCOMPLEX_FILE_LOADER_H_
#define _EASYCOMPLEX_FILE_LOADER_H_

#include <string>
#include <vector>

namespace ecomplex
{

class Symbol;

class FileLoader
{
public:
	static void Load(const std::string& filepath, Symbol* complex);

	static void LoadChildren(const std::string& filepath, std::vector<std::string>& children);

}; // FileLoader

}

#endif // _EASYCOMPLEX_FILE_LOADER_H_