#ifndef _EASYICON_FILE_IO_H_
#define _EASYICON_FILE_IO_H_

#include "Icon.h"

#include <memory>

namespace eicon
{

class FileIO
{
public:
	static void StoreToFile(const char* filename, const Icon& icon);
	static std::unique_ptr<s2::Icon> LoadFromFile(const char* filename);

}; // FileIO

}

#endif // _EASYICON_FILE_IO_H_