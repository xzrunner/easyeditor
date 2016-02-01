#ifndef _EASYSHAPE_SHAPE_FACTORY_H_
#define _EASYSHAPE_SHAPE_FACTORY_H_

namespace ee { class Shape; }

#include <json/json.h>

namespace eshape
{

class ShapeFactory
{
public:
	static ee::Shape* CreateShapeFromFile(const Json::Value& value,
		const std::string& dir);

}; // ShapeFactory

}

#endif // _EASYSHAPE_SHAPE_FACTORY_H_