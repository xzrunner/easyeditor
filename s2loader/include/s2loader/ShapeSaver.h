#ifndef _S2LOADER_SHAPE_SAVER_H_
#define _S2LOADER_SHAPE_SAVER_H_

#include <json/json.h>

#include <memory>

namespace s2 { class Shape; class PolygonShape; }

namespace s2loader
{

class ShapeSaver
{
public:
	static void Store(const std::shared_ptr<const s2::Shape>& shape, Json::Value& val);

private:
	static void StorePolygon(const std::shared_ptr<const s2::PolygonShape>& polygon, Json::Value& val);

}; // ShapeSaver

}

#endif // _S2LOADER_SHAPE_SAVER_H_
