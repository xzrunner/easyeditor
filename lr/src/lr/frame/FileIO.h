#ifndef _LR_FILE_IO_H_
#define _LR_FILE_IO_H_

#include <json/json.h>
#include <drag2d.h>

namespace lr
{

class StagePanel;

class FileIO
{
public:
	static void load(const char* filename, StagePanel* stage,
		d2d::LibraryPanel* library);
	static void store(const char* filename, StagePanel* stage);

}; // FileIO

}

#endif // _LR_FILE_IO_H_