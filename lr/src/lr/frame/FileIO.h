#ifndef _LR_FILE_IO_H_
#define _LR_FILE_IO_H_

#include <json/json.h>


namespace lr
{

class LibraryPanel;
class StagePanel;
class Layer;

class FileIO
{
public:
	static void Load(const char* filename, LibraryPanel* library, 
		StagePanel* stage, ee::GroupTreePanel* grouptree);
	static void Store(const char* filename, LibraryPanel* library,
		StagePanel* stage, ee::GroupTreePanel* grouptree);

private:
	static void LoadLayers(const Json::Value& value, StagePanel* stage,
		LibraryPanel* library, const std::string& dir);
	static void StoreLayers(Json::Value& value, const std::vector<Layer*>& layers, 
		const std::string& dir);

}; // FileIO

}

#endif // _LR_FILE_IO_H_