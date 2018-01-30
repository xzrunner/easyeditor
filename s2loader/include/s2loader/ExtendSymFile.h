#ifndef _S2LOADER_EXTEND_SYM_FILE_H_
#define _S2LOADER_EXTEND_SYM_FILE_H_

#include <cu/cu_macro.h>

#include <json/json.h>

namespace s2loader
{

enum ExtendSymType
{
	SYM_UNKNOWN,
	SYM_SPINE,
	SYM_BODYMOVIN,
};

class ExtendSymFile
{
public:
	static int GetType(const Json::Value& val);

}; // ExtendSymFile

}

#endif // _S2LOADER_EXTEND_SYM_FILE_H_
