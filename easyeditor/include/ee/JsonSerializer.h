#ifndef _EASYEDITOR_JSON_SERIALIZER_H_
#define _EASYEDITOR_JSON_SERIALIZER_H_

#include "Color.h"

#include <SM_Vector.h>

#include <json/value.h>

#include <vector>

namespace ee
{

class Rect;

class JsonSerializer
{
public:
	static void Store(const std::vector<sm::vec2>& points, Json::Value& value);
	static void Load(const Json::Value& value, std::vector<sm::vec2>& points);

	static void Store(const std::vector<Colorf>& colors, Json::Value& value);
	static void Load(const Json::Value& value, std::vector<Colorf>& colors);

	static void Store(const Rect& r, Json::Value& value);
	static void Load(const Json::Value& value, Rect& r);

	static void Store(const Colorf& col, Json::Value& value);
	static void Load(const Json::Value& value, Colorf& col);

}; // JsonSerializer

}

#endif // _EASYEDITOR_JSON_SERIALIZER_H_