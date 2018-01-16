#ifndef _EASYCOMPLEX_FILE_STORER_H_
#define _EASYCOMPLEX_FILE_STORER_H_

#include <ee/Sprite.h>

#include <cu/cu_stl.h>

namespace ecomplex
{

class Symbol;

class FileStorer
{
public:
	static void Store(const std::string& filepath, const Symbol& sym, 
		const std::string& dir, bool store_history);

}; // FileStorer

}

#endif // _EASYCOMPLEX_FILE_STORER_H_