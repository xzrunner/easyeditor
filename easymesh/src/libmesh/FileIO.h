#ifndef _EASYMESH_FILE_IO_H_
#define _EASYMESH_FILE_IO_H_

namespace emesh
{

class Symbol;
class FileIO
{
public:
	static void Store(const char* filepath, const Symbol* sym);
	static void Load(const char* filepath, Symbol* sym);
}; // FileIO

}

#endif // _EASYMESH_FILE_IO_H_