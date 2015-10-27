#ifndef _EASYUI_LIST_FILE_IO_H_
#define _EASYUI_LIST_FILE_IO_H_

#include <drag2d.h>

namespace eui
{
namespace list
{

class UIList;

class FileIO
{
public:
//	static void Load(const char* filename);
	static void Store(const char* filename, const UIList* list);

}; // FileIO

}
}

#endif // _EASYUI_LIST_FILE_IO_H_