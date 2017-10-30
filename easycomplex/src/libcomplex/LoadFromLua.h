#ifndef _EASYCOMPLEX_LOAD_FROM_LUA_H_
#define _EASYCOMPLEX_LOAD_FROM_LUA_H_

#include <ee/Image.h>

#include <json/json.h>

namespace ecomplex
{

class Symbol;

class LoadFromLua
{
public:
	static void Load(const Json::Value& value, const std::string& dir, Symbol& complex);

private:
	static void LoadImages(const std::string& name, std::vector<ee::ImagePtr>& images);

}; // LoadFromLua

}

#endif // _EASYCOMPLEX_LOAD_FROM_LUA_H_