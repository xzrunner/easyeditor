#ifndef _EASYMAP_FILEIO_H_
#define _EASYMAP_FILEIO_H_

namespace emap
{

class Task;

class FileIO
{
public:
	static void Load(const Task* task, const char* filename);
	static void Store(const Task* task, const char* filename);

}; // FileIO

}

#endif // _EASYMAP_FILEIO_H_