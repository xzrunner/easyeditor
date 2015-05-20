#ifndef _EASYUI_FILE_IO_H_
#define _EASYUI_FILE_IO_H_

#include <json/json.h>
#include <drag2d.h>

namespace eui
{

class StagePanel;

class FileIO
{
public:
	static void Load(const char* filename, StagePanel* stage,
		d2d::LibraryPanel* library);
	static void Store(const char* filename, StagePanel* stage);

}; // FileIO

}

#endif // _EASYUI_FILE_IO_H_