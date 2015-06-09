#ifndef _DRAG2D_JSON_IO_H_
#define _DRAG2D_JSON_IO_H_

#include <vector>
#include <json/value.h>

#include "common/Vector.h"
#include "common/Color.h"
#include "common/Rect.h"

namespace d2d
{

class JsonIO
{
public:
	static void Store(const std::vector<Vector>& points, Json::Value& value);
	static void Load(const Json::Value& value, std::vector<Vector>& points);

	static void Store(const std::vector<Colorf>& colors, Json::Value& value);
	static void Load(const Json::Value& value, std::vector<Colorf>& colors);

	static void Store(const d2d::Rect& r, Json::Value& value);
	static void Load(const Json::Value& value, d2d::Rect& r);

	static void Store(const Colorf& col, Json::Value& value);
	static void Load(const Json::Value& value, Colorf& col);

}; // JsonIO

}

#endif // _DRAG2D_JSON_IO_H_