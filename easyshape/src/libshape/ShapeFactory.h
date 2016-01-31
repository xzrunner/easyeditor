#ifndef _LIBSHAPE_SHAPE_FACTORY_H_
#define _LIBSHAPE_SHAPE_FACTORY_H_


#include <json/json.h>

namespace libshape
{

class ShapeFactory
{
public:
	static ee::Shape* CreateShapeFromFile(const Json::Value& value,
		const std::string& dir);

}; // ShapeFactory

}

#endif // _LIBSHAPE_SHAPE_FACTORY_H_