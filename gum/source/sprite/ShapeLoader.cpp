#include "ShapeLoader.h"
#include "JsonSerializer.h"
#include "FilepathHelper.h"
#include "TexturePolygon.h"

#include <sprite2/PolygonShape.h>
#include <sprite2/ColorPolygon.h>

namespace gum
{

s2::Shape* ShapeLoader::LoadShape(const Json::Value& val, const std::string& dir)
{
	std::string type = val["type"].asString();

	if (type == "polygon") {
		return LoadPolygon(val, dir);
	}

	return NULL;
}

s2::PolygonShape* ShapeLoader::LoadPolygon(const Json::Value& val, const std::string& dir)
{
	s2::PolygonShape* poly = new s2::PolygonShape;

	std::vector<sm::vec2> vertices;
	JsonSerializer::Load(val["vertices"], vertices);
	poly->SetVertices(vertices);

	const Json::Value& mt_val = val["material"];
	poly->SetPolygon(LoadPolyMaterial(mt_val, dir, vertices));

	return poly;
}

s2::Polygon* ShapeLoader::LoadPolyMaterial(const Json::Value& val, const std::string& dir,
										   const std::vector<sm::vec2>& vertice)
{
	s2::Polygon* poly = NULL;

	std::string type = val["type"].asString();
	if (type == "color")
	{
		s2::Color col;
		col.FromRGBA(val["color"].asUInt());
		poly = new s2::ColorPolygon(col);
	}
	else if (type == "texture") 
	{
		std::string path = val["texture path"].asString();
		path = FilepathHelper::Absolute(dir, path);
		poly = new TexturePolygon(path);
	}

	if (poly) {
		poly->SetOutline(vertice);
		poly->Build();
	}

	return poly;
}

}