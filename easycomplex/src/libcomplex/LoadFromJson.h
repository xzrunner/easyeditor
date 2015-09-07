#ifndef _LIBCOMPLEX_LOAD_FROM_JSON_H_
#define _LIBCOMPLEX_LOAD_FROM_JSON_H_

#include <json/json.h>

namespace ecomplex
{

class Symbol;

class LoadFromJson
{
public:
	static void Load(const Json::Value& value, const std::string& dir, 
		Symbol* complex);

}; // LoadFromJson

}

#endif // _LIBCOMPLEX_LOAD_FROM_JSON_H_