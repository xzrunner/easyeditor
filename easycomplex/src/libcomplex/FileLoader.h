#ifndef _EASYCOMPLEX_FILE_LOADER_H_
#define _EASYCOMPLEX_FILE_LOADER_H_

#include <string>

namespace ecomplex
{

class Symbol;

class FileLoader
{
public:
	static void Load(const std::string& filepath, Symbol* complex);

}; // FileLoader

}

#endif // _EASYCOMPLEX_FILE_LOADER_H_