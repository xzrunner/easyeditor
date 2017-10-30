#ifndef _SG_FILE_IO_H_
#define _SG_FILE_IO_H_

#include <json/json.h>

namespace ee { class Sprite; class Symbol; }

namespace sg
{

class StagePanel;

class FileIO
{
public:
	static void load(const char* filename, StagePanel* stage);
	static void store(const char* filename, StagePanel* stage);

private:
	static ee::SprPtr load(const Json::Value& value, StagePanel* stage, const std::string& dir);
	static Json::Value store(const ee::SprConstPtr& spr, StagePanel* stage, const std::string& dir);

	static void SetSymbolUserData(const ee::SymPtr& sym);

}; // FileIO

}

#endif // _SG_FILE_IO_H_