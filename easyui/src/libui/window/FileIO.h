#ifndef _EASYUI_WINDOW_FILE_IO_H_
#define _EASYUI_WINDOW_FILE_IO_H_

#include <json/json.h>

#include <vector>
#include <string>

namespace ee { class Sprite; }

namespace eui
{
namespace window
{

class Symbol;

class FileIO
{
public:
	static void Store(const char* filepath, const Symbol* sym);
	static void Load(const char* filepath, Symbol* sym);

	static void FetchSprites(const std::string& filepath, std::vector<ee::Sprite*>& sprs);

private:
	static std::string StoreWrapper(const std::string& filepath,
		const std::string& name, const std::vector<ee::Sprite*>& sprs);

	static void LoadSprites(const Json::Value& val, Symbol* sym, const std::string& dir);

	static void StoreRefs(Json::Value& val, const Symbol* sym, const std::string& dir);
	static void LoadRefs(const Json::Value& val, Symbol* sym, const std::string& dir);

}; // FileIO

}
}

#endif // _EASYUI_WINDOW_FILE_IO_H_