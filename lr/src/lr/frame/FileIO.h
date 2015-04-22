#ifndef _LR_FILE_IO_H_
#define _LR_FILE_IO_H_

#include <json/json.h>
#include <drag2d.h>

namespace lr
{

class LibraryPanel;
class StagePanel;

class FileIO
{
public:
	static void load(const char* filename, StagePanel* stage,
		LibraryPanel* library);
	static void store(const char* filename, StagePanel* stage,
		LibraryPanel* library);

private:
	static void LoadSprites(const Json::Value& value, StagePanel* stage, 
		const std::string& dir);
	static void StoreSprites(Json::Value& value, StagePanel* stage, 
		const std::string& dir);

}; // FileIO

}

#endif // _LR_FILE_IO_H_