#include "s2loader/ShapeSaver.h"

#include <sprite2/Shape.h>
#include <sprite2/ShapeType.h>
#include <sprite2/PolygonShape.h>
#include <sprite2/Polygon.h>
#include <sprite2/PolyType.h>
#include <sprite2/ColorPolygon.h>
#include <gum/JsonSerializer.h>

namespace s2loader
{

void ShapeSaver::Store(const std::shared_ptr<const s2::Shape>& shape, Json::Value& val)
{
	switch (shape->Type())
	{
	case s2::SHAPE_POLYGON:
		StorePolygon(S2_VI_PTR_DOWN_CAST<const s2::PolygonShape>(shape), val);
		break;
	}
}

void ShapeSaver::StorePolygon(const std::shared_ptr<const s2::PolygonShape>& poly_shape, Json::Value& val)
{
	val["type"] = "polygon";
		
	auto& vertices = poly_shape->GetVertices();
	 gum::JsonSerializer::Store(vertices, val["vertices"]);

	Json::Value& mt_val = val["material"];

	const s2::Polygon* poly = poly_shape->GetPolygon().get();
	switch (poly->Type())
	{
	case s2::POLY_COLOR:
		{
			mt_val["type"] = "color";
			auto color_poly = S2_VI_DOWN_CAST<const s2::ColorPolygon*>(poly);
			mt_val["color"] = color_poly->GetColor().ToRGBA();
		}
		break;
	case s2::POLY_TEXTURE:
		mt_val["type"] = "texture";
		break;
	}
}

}