#ifndef _GUM_SHAPE_LOADER_H_
#define _GUM_SHAPE_LOADER_H_

#include <SM_Vector.h>

#include <json/json.h>

namespace s2 { class Shape; class PolygonShape; class Polygon; }

namespace gum
{

class ShapeLoader
{
public:
	static s2::Shape* LoadShape(const Json::Value& val, const std::string& dir);

private:
	static s2::PolygonShape* LoadPolygon(const Json::Value& val, const std::string& dir);

	static s2::Polygon* LoadPolyMaterial(const Json::Value& val, const std::string& dir,
		const std::vector<sm::vec2>& vertice);

}; // ShapeLoader

}

#endif // _GUM_SHAPE_LOADER_H_