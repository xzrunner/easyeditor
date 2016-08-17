#ifndef _EASYSCALE9_FILE_SAVER_H_
#define _EASYSCALE9_FILE_SAVER_H_

namespace escale9
{

class Symbol;

class FileSaver
{
public:
	static void Store(const char* filepath, const Symbol& sym);

}; // FileSaver

}

#endif // _EASYSCALE9_FILE_SAVER_H_