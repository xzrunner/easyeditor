#ifndef _LR_FILE_IO_H_
#define _LR_FILE_IO_H_

#include <json/json.h>

namespace ee { class GroupTreePanel; }

namespace lr
{

class LibraryPanel;
class StagePanel;
class Layer;

class FileIO
{
public:
	static void Load(const char* filename, LibraryPanel* library, 
		StagePanel* stage, ee::GroupTreePanel* grouptree, std::string& err_log);
	static void Store(const char* filename, LibraryPanel* library,
		StagePanel* stage, ee::GroupTreePanel* grouptree);

private:
	static void LoadLayers(const Json::Value& value, StagePanel* stage,
		LibraryPanel* library, const std::string& dir, std::string& err_log);
	static void StoreLayers(Json::Value& value, const std::vector<Layer*>& layers, 
		const std::string& dir);

}; // FileIO

}

#endif // _LR_FILE_IO_H_