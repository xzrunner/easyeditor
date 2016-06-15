#ifndef _EASYMASK_FILE_IO_H_
#define _EASYMASK_FILE_IO_H_

namespace emask
{

class Symbol;
class FileIO
{
public:
	static void Store(const char* filepath, const Symbol* sym);
	static void Load(const char* filepath, Symbol* sym);

}; // FileIO

}

#endif // _EASYMASK_FILE_IO_H_