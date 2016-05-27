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

ee::Shape* ShapeFactory::CreateShapeFromFile(const Json::Value& value, 
											   const std::string& dir)
{
	ee::Shape* shape = NULL;

	std::string desc = value["type"].asString();
	ShapeType type = get_shape_type(desc);
	switch (type)
	{
	case ST_POINT:
		shape = new PointShape;
		break;
	case ST_BEZIER:
		shape = new BezierShape;
		break;
	case ST_CHAIN:
		shape = new ChainShape;
		break;
	case ST_CIRCLE:
		shape = new CircleShape;
		break;
// 	case ST_COMPLEX_POLYGON:
// 		shape = new ComplexPolygonShape;
// 		break;
	case ST_COSINE_CURVE:
		shape = new CosineShape;
		break;
	case ST_POLYGON:
		shape = new PolygonShape;
		break;
	case ST_RECT:
		shape = new RectShape;
		break;
	}

	if (shape) {
		shape->LoadFromFile(value, dir);
	}

	return shape;
}

}