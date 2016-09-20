#ifndef _EASYSHAPE_FILE_IO_H_
#define _EASYSHAPE_FILE_IO_H_

#include <json/json.h>

namespace ee { class Shape; class Symbol; }

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
	static ee::Shape* LoadFromFile(const char* filename, std::string& bg_filepath);
	static ee::Shape* LoadFromFile(const char* filename, ee::Symbol*& bg);
	static void StoreToFile(const char* filename, const ee::Shape* shape, const ee::Symbol* bg);

	static ee::Shape* LoadShape(const std::string& dir, const Json::Value& value);
	static Json::Value StoreShape(const std::string& dir, const ee::Shape* shape);

private:
	static ee::Shape* LoadBezier(const Json::Value& value);
	static ee::Shape* LoadPolygon(const std::string& dir, const Json::Value& value);
	static ee::Shape* LoadChain(const Json::Value& value);
	static ee::Shape* LoadRect(const Json::Value& value);
	static ee::Shape* LoadCircle(const Json::Value& value);

}; // FileIO

}

#endif // _EASYSHAPE_FILE_IO_H_