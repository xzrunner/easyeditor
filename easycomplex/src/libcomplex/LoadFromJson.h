#ifndef _EASYCOMPLEX_LOAD_FROM_JSON_H_
#define _EASYCOMPLEX_LOAD_FROM_JSON_H_

#include <json/json.h>

namespace ee { class Sprite; }

namespace ecomplex
{

class Symbol;

class LoadFromJson
{
public:
	static void Load(const std::string& filepath, const Json::Value& value, 
		const std::string& dir, Symbol* complex);

	static void CreateActionsFromTag(Symbol* sym);

private:
	static ee::Sprite* LoadSprite(const Json::Value& val, const std::string& dir);

	static void InitActions(Symbol* sym, const Json::Value& val);

}; // LoadFromJson

}

#endif // _EASYCOMPLEX_LOAD_FROM_JSON_H_