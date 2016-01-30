#ifndef _SG_FILE_IO_H_
#define _SG_FILE_IO_H_

#include <json/json.h>
#include <drag2d.h>

namespace sg
{

class StagePanel;

class FileIO
{
public:
	static void load(const char* filename, StagePanel* stage);
	static void store(const char* filename, StagePanel* stage);

private:
	static d2d::Sprite* load(const Json::Value& value, StagePanel* stage, const std::string& dir);
	static Json::Value store(const d2d::Sprite* sprite, StagePanel* stage, const std::string& dir);

	static void SetSymbolUserData(d2d::Symbol* symbol);

}; // FileIO

}

#endif // _SG_FILE_IO_H_