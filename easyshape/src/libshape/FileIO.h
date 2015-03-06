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
	static void LoadFromFile(const char* filename, std::vector<d2d::IShape*>& shapes, 
		std::string& bg_filepath);
	static void LoadFromFile(const char* filename, std::vector<d2d::IShape*>& shapes, 
		d2d::ISymbol*& bg);
	static void StoreToFile(const char* filename, const std::vector<d2d::IShape*>& shapes, 
		const d2d::ISymbol* bg);

	static d2d::IShape* LoadShape(const std::string& dir, const Json::Value& value);
	static Json::Value StoreShape(const std::string& dir, d2d::IShape* shape);

private:
	static d2d::IShape* LoadBezier(const Json::Value& value);
	static d2d::IShape* LoadPolygon(const std::string& dir, const Json::Value& value);
	static d2d::IShape* LoadChain(const Json::Value& value);
	static d2d::IShape* LoadRect(const Json::Value& value);
	static d2d::IShape* LoadCircle(const Json::Value& value);

	static Json::Value StoreBezier(const BezierShape* bezier);
	static Json::Value StorePolygon(const std::string& dir, const PolygonShape* poly);
	static Json::Value StoreChain(const ChainShape* chain);
	static Json::Value StoreRect(const RectShape* rect);
	static Json::Value StoreCircle(const CircleShape* circle);

}; // FileIO

}

#endif // _LIBSHAPE_FILE_IO_H_