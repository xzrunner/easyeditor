#ifndef _EASYCOMPLEX_LOAD_FROM_JSON_H_
#define _EASYCOMPLEX_LOAD_FROM_JSON_H_

#include <json/json.h>

namespace ecomplex
{

class Symbol;

class LoadFromJson
{
public:
	static void Load(const std::string& filepath, const Json::Value& value, 
		const std::string& dir, Symbol* complex);

private:
	static void InitActions(Symbol* sym, const Json::Value& val);
	static void CreateActionsFromTag(Symbol* sym);

}; // LoadFromJson

}

#endif // _EASYCOMPLEX_LOAD_FROM_JSON_H_