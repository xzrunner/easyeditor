#include "s2loader/ShapeLoader.h"

#include <sprite2/PolygonShape.h>
#include <sprite2/ColorPolygon.h>
#include <gum/FilepathHelper.h>
#include <gum/JsonSerializer.h>
#include <gum/TexturePolygon.h>

namespace s2loader
{

std::unique_ptr<s2::Shape> ShapeLoader::LoadShape(const Json::Value& val, const CU_STR& dir)
{
	CU_STR type = val["type"].asString().c_str();

	if (type == "polygon") {
		return LoadPolygon(val, dir);
	}

	return nullptr;
}

std::unique_ptr<s2::PolygonShape> ShapeLoader::LoadPolygon(const Json::Value& val, const CU_STR& dir)
{
	auto poly = std::make_unique<s2::PolygonShape>();

	CU_VEC<sm::vec2> vertices;
	 gum::JsonSerializer::Load(val["vertices"], vertices);
	poly->SetVertices(vertices);

	const Json::Value& mt_val = val["material"];
	poly->SetPolygon(LoadPolyMaterial(mt_val, dir, vertices));

	return poly;
}

std::unique_ptr<s2::Polygon> ShapeLoader::LoadPolyMaterial(const Json::Value& val, const CU_STR& dir,
	                                                       const CU_VEC<sm::vec2>& vertice)
{
	std::unique_ptr<s2::Polygon> poly;

	CU_STR type = val["type"].asString().c_str();
	if (type == "color")
	{
		pt2::Color col;
		col.FromRGBA(val["color"].asUInt());
		poly = std::make_unique<s2::ColorPolygon>(col);
	}
	else if (type == "texture") 
	{
		CU_STR path = val["texture path"].asString().c_str();
		path = gum::FilepathHelper::Absolute(dir, path);
		poly = std::make_unique<gum::TexturePolygon>(path);
	}

	if (poly) {
		poly->SetOutline(vertice);
		poly->Build();
	}

	return poly;
}

}