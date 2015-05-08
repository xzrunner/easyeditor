#ifndef _LIBSHAPE_SHAPE_FACTORY_H_
#define _LIBSHAPE_SHAPE_FACTORY_H_

#include <drag2d.h>
#include <json/json.h>

namespace libshape
{

class ShapeFactory
{
public:
	static d2d::IShape* CreateShapeFromFile(const Json::Value& value,
		const std::string& dir);

}; // ShapeFactory

}

#endif // _LIBSHAPE_SHAPE_FACTORY_H_