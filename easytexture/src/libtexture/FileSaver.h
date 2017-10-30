#ifndef _EASYTEXTURE_FILE_SAVER_H_
#define _EASYTEXTURE_FILE_SAVER_H_

#include <memory>

namespace etexture
{

class Symbol;

class FileSaver
{
public:
	static void Store(const char* filepath, const std::shared_ptr<Symbol>& sym);

}; // FileSaver

}

#endif // _EASYTEXTURE_FILE_SAVER_H_