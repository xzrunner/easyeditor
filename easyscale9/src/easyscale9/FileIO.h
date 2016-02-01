#ifndef _EASYSCALE9_FILE_IO_H_
#define _EASYSCALE9_FILE_IO_H_

#include <json/json.h>

namespace ee { class LibraryPanel; class Sprite; class MultiSpritesImpl; }

namespace escale9
{

class StagePanel;
class ToolbarPanel;

class FileIO
{
public:
	static void Load(const char* filename, ee::LibraryPanel* library, 
		ee::MultiSpritesImpl* stage, ToolbarPanel* toolbar);
	static void Store(const char* filename, StagePanel* stage, 
		ToolbarPanel* toolbar);

private:
	static ee::Sprite* Load(const Json::Value& value, const std::string& dir);
	static Json::Value Store(ee::Sprite* sprite, const std::string& dir);
	static Json::Value StoreNew(ee::Sprite* sprite, const std::string& dir);

}; // FileIO

}

#endif // _EASYSCALE9_FILE_IO_H_