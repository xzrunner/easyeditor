#include "s2loader/ExtendSymFile.h"

namespace s2loader
{

int ExtendSymFile::GetType(const Json::Value& val)
{
	int ret = SYM_UNKNOWN;

	if (!val.isArray() && 
		val.isMember("skeleton") && 
		!val["skeleton"].isArray() && 
		val["skeleton"].isMember("spine")) 
	{
		ret = SYM_SPINE;
	} 
	else if (
		!val.isArray() && 
		val.isMember("assets") && 
		val["assets"].isArray() &&
		val.isMember("layers") &&
		val["layers"].isArray() &&
		val.isMember("fr") &&
		val.isMember("w") &&
		val.isMember("h") &&
		val.isMember("v"))
	{
		ret = SYM_BODYMOVIN;
	}

	return ret;
}

}