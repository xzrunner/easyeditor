#ifndef _LIBCOMPLEX_FILE_LOADER_H_
#define _LIBCOMPLEX_FILE_LOADER_H_

#include <drag2d.h>
#include <easycoco.h>

namespace ecomplex
{

class Symbol;

class FileLoader
{
public:
	static void Load(const std::string& filepath, Symbol* complex);

}; // FileLoader

}

#endif // _LIBCOMPLEX_FILE_LOADER_H_