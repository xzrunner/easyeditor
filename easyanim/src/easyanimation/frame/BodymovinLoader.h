#ifndef _EASYANIM_BODYMOVIN_LOADER_H_
#define _EASYANIM_BODYMOVIN_LOADER_H_

#include <json/json.h>

namespace eanim
{

class BodymovinLoader
{
public:
	static void Load(const Json::Value& val, const std::string& dir);

}; // BodymovinLoader

}

#endif // _EASYANIM_BODYMOVIN_LOADER_H_