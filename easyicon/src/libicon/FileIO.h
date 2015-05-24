#ifndef _EASYICON_FILE_IO_H_
#define _EASYICON_FILE_IO_H_

namespace eicon
{

class Icon;

class FileIO
{
public:
	static void StoreToFile(const char* filename, const Icon* icon);
	static Icon* LoadFromFile(const char* filename);

}; // FileIO

}

#endif // _EASYICON_FILE_IO_H_