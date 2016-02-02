#ifndef _EASYMESH_FILE_IO_H_
#define _EASYMESH_FILE_IO_H_

namespace emesh
{

class Symbol;
class FileIO
{
public:
	static void Store(const char* filepath, const Symbol* symbol);
	static void Load(const char* filepath, Symbol* symbol);
}; // FileIO

}

#endif // _EASYMESH_FILE_IO_H_