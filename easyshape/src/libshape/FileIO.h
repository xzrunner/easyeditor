#ifndef _LIBSHAPE_FILE_IO_H_
#define _LIBSHAPE_FILE_IO_H_



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
	static void LoadFromFile(const char* filename, std::vector<ee::Shape*>& shapes, 
		std::string& bg_filepath);
	static void LoadFromFile(const char* filename, std::vector<ee::Shape*>& shapes, 
		ee::Symbol*& bg);
	static void StoreToFile(const char* filename, const std::vector<ee::Shape*>& shapes, 
		const ee::Symbol* bg);

	static ee::Shape* LoadShape(const std::string& dir, const Json::Value& value);
	static Json::Value StoreShape(const std::string& dir, ee::Shape* shape);

private:
	static ee::Shape* LoadBezier(const Json::Value& value);
	static ee::Shape* LoadPolygon(const std::string& dir, const Json::Value& value);
	static ee::Shape* LoadChain(const Json::Value& value);
	static ee::Shape* LoadRect(const Json::Value& value);
	static ee::Shape* LoadCircle(const Json::Value& value);

}; // FileIO

}

#endif // _LIBSHAPE_FILE_IO_H_