#ifndef _LIBSHAPE_FILE_IO_H_
#define _LIBSHAPE_FILE_IO_H_

#include <drag2d.h>

namespace libshape
{

class BezierShape;
class PolygonShape;
class ChainShape;
class RectShape;
class CircleShape;

class FileIO
{
public:
	static void LoadFromFile(const char* filename, std::vector<d2d::Shape*>& shapes, 
		std::string& bg_filepath);
	static void LoadFromFile(const char* filename, std::vector<d2d::Shape*>& shapes, 
		d2d::Symbol*& bg);
	static void StoreToFile(const char* filename, const std::vector<d2d::Shape*>& shapes, 
		const d2d::Symbol* bg);

	static d2d::Shape* LoadShape(const std::string& dir, const Json::Value& value);
	static Json::Value StoreShape(const std::string& dir, d2d::Shape* shape);

private:
	static d2d::Shape* LoadBezier(const Json::Value& value);
	static d2d::Shape* LoadPolygon(const std::string& dir, const Json::Value& value);
	static d2d::Shape* LoadChain(const Json::Value& value);
	static d2d::Shape* LoadRect(const Json::Value& value);
	static d2d::Shape* LoadCircle(const Json::Value& value);

}; // FileIO

}

#endif // _LIBSHAPE_FILE_IO_H_