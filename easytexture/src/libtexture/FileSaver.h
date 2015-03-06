#ifndef _EASYTEXTURE_FILE_SAVER_H_
#define _EASYTEXTURE_FILE_SAVER_H_

namespace etexture
{

class Symbol;

class FileSaver
{
public:
	static void Store(const char* filepath, const Symbol* symbol);

}; // FileSaver

}

#endif // _EASYTEXTURE_FILE_SAVER_H_