#ifndef _DRAG2D_JSON_TOOLS_H_
#define _DRAG2D_JSON_TOOLS_H_

#include <vector>
#include <json/value.h>

#include "common/Vector.h"

namespace d2d
{

class JsonTools
{
public:
	static void store(const std::vector<Vector>& points, Json::Value& value);
	static void load(const Json::Value& value, std::vector<Vector>& points);

}; // JsonTools

}

#endif // _DRAG2D_JSON_TOOLS_H_