#include "ShapeFactory.h"
#include "ShapeType.h"

#include "PointShape.h"
#include "BezierShape.h"
#include "ChainShape.h"
#include "CircleShape.h"
//#include "ComplexPolygonShape.h"
#include "CosineShape.h"
#include "PolygonShape.h"
#include "RectShape.h"

namespace eshape
{

ee::ShapePtr ShapeFactory::CreateShapeFromFile(const Json::Value& value, 
											   const std::string& dir)
{
	ee::ShapePtr shape = nullptr;

	std::string desc = value["type"].asString();
	ShapeType type = get_shape_type(desc);
	switch (type)
	{
	case ST_POINT:
		shape = std::make_unique<PointShape>();
		break;
	case ST_BEZIER:
		shape = std::make_unique<BezierShape>();
		break;
	case ST_CHAIN:
		shape = std::make_unique<ChainShape>();
		break;
	case ST_CIRCLE:
		shape = std::make_unique<CircleShape>();
		break;
// 	case ST_COMPLEX_POLYGON:
// 		shape = std::make_unique<ComplexPolygonShape>();
// 		break;
	case ST_COSINE_CURVE:
		shape = std::make_unique<CosineShape>();
		break;
	case ST_POLYGON:
		shape = std::make_unique<PolygonShape>();
		break;
	case ST_RECT:
		shape = std::make_unique<RectShape>();
		break;
	}

	if (shape) {
		shape->LoadFromFile(value, dir);
	}

	return shape;
}

}