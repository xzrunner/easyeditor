#ifndef _EASYSHAPE_FILE_IO_H_
#define _EASYSHAPE_FILE_IO_H_

#include <ee/Symbol.h>
#include <ee/Shape.h>

#include <json/json.h>

namespace eshape
{

class BezierShape;
class PolygonShape;
class ChainShape;
class RectShape;
class CircleShape;

class FileIO
{
public:
	static ee::ShapePtr LoadFromFile(const char* filename, std::string& bg_filepath);
	static ee::ShapePtr LoadFromFile(const char* filename, ee::SymPtr& bg);
	static void StoreToFile(const char* filename, const ee::Shape& shape, const ee::SymPtr& bg);

	static ee::ShapePtr LoadShape(const std::string& dir, const Json::Value& value);
	static Json::Value StoreShape(const std::string& dir, const ee::Shape& shape);

private:
	static ee::ShapePtr LoadBezier(const Json::Value& value);
	static ee::ShapePtr LoadPolygon(const std::string& dir, const Json::Value& value);
	static ee::ShapePtr LoadChain(const Json::Value& value);
	static ee::ShapePtr LoadRect(const Json::Value& value);
	static ee::ShapePtr LoadCircle(const Json::Value& value);

}; // FileIO

}

#endif // _EASYSHAPE_FILE_IO_H_