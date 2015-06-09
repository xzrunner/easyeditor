#ifndef _EASYSHADOW_FILE_IO_H_
#define _EASYSHADOW_FILE_IO_H_

namespace eshadow
{

class Symbol;

class FileIO
{
public:
	static void StoreToFile(const char* filepath, const Symbol* symbol);
	static void LoadFromFile(const char* filepath, Symbol* symbol);

}; // FileIO

}

#endif // _EASYSHADOW_FILE_IO_H_