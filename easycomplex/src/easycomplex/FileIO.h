#ifndef _EASYCOMPLEX_FILEIO_H_
#define _EASYCOMPLEX_FILEIO_H_

namespace ecomplex
{

class Task;
class FileIO
{
public:
	static void LoadFromFile(const Task* task, const char* filename);
	static void StoreToFile(const Task* task, const char* filename);

}; // FileIO

}

#endif // _EASYCOMPLEX_FILEIO_H_