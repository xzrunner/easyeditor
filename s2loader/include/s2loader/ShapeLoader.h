#ifndef _S2LOADER_SHAPE_LOADER_H_
#define _S2LOADER_SHAPE_LOADER_H_

#include <SM_Vector.h>
#include <cu/cu_stl.h>

#include <json/json.h>

#include <memory>

namespace s2 { class Shape; class PolygonShape; class Polygon; }

namespace s2loader
{

class ShapeLoader
{
public:
	static std::unique_ptr<s2::Shape> LoadShape(const Json::Value& val, const CU_STR& dir);

private:
	static std::unique_ptr<s2::PolygonShape> LoadPolygon(const Json::Value& val, const CU_STR& dir);

	static std::unique_ptr<s2::Polygon> LoadPolyMaterial(const Json::Value& val, const CU_STR& dir,
		const CU_VEC<sm::vec2>& vertice);

}; // ShapeLoader

}

#endif // _S2LOADER_SHAPE_LOADER_H_
