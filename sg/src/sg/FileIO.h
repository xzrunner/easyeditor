#ifndef _SG_FILE_IO_H_
#define _SG_FILE_IO_H_

#include <json/json.h>


namespace sg
{

class StagePanel;

class FileIO
{
public:
	static void load(const char* filename, StagePanel* stage);
	static void store(const char* filename, StagePanel* stage);

private:
	static ee::Sprite* load(const Json::Value& value, StagePanel* stage, const std::string& dir);
	static Json::Value store(const ee::Sprite* sprite, StagePanel* stage, const std::string& dir);

	static void SetSymbolUserData(ee::Symbol* symbol);

}; // FileIO

}

#endif // _SG_FILE_IO_H_