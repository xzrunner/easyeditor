#ifndef _LIBCOMPLEX_FILE_LOADER_H_
#define _LIBCOMPLEX_FILE_LOADER_H_

#include <string>

namespace ecomplex
{

class Symbol;

class FileLoader
{
public:
	static void Load(const std::string& filepath, Symbol* symbol);

}; // FileLoader

}

#endif // _LIBCOMPLEX_FILE_LOADER_H_