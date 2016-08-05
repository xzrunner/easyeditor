#ifndef _EASYEDITOR_JSON_SERIALIZER_H_
#define _EASYEDITOR_JSON_SERIALIZER_H_

#include <SM_Vector.h>
#include <SM_Rect.h>
#include <sprite2/Color.h>

#include <json/value.h>

#include <vector>

namespace ee
{

class JsonSerializer
{
public:
	static void Store(const std::vector<sm::vec2>& points, Json::Value& value);
	static void Load(const Json::Value& value, std::vector<sm::vec2>& points);

	static void Store(const std::vector<s2::Color>& colors, Json::Value& value);
	static void Load(const Json::Value& value, std::vector<s2::Color>& colors);

	static void Store(const sm::rect& r, Json::Value& value);
	static void Load(const Json::Value& value, sm::rect& r);

	static void Store(const s2::Color& col, Json::Value& value);
	static void Load(const Json::Value& value, s2::Color& col);

	static void Store(const uint8_t rgba[4], Json::Value& value);
	static void Load(const Json::Value& value, uint8_t rgba[4]);

}; // JsonSerializer

}

#endif // _EASYEDITOR_JSON_SERIALIZER_H_