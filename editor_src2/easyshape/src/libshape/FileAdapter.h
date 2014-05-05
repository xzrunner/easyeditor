#ifndef _LIBSHAPE_FILE_ADAPTER_H_
#define _LIBSHAPE_FILE_ADAPTER_H_

#include <drag2d.h>

namespace libshape
{

class BezierShape;
class PolygonShape;
class ChainShape;
class RectShape;
class CircleShape;

class FileAdapter : public d2d::IFileAdapter
{
public:
	FileAdapter(std::vector<d2d::IShape*>& shapes);

	virtual void load(const char* filename);
	virtual void store(const char* filename);

	static d2d::IShape* loadShape(const Json::Value& value);
	static Json::Value store(d2d::IShape* shape);

private:
	static d2d::IShape* loadBezier(const Json::Value& value);
	static d2d::IShape* loadPolygon(const Json::Value& value);
	static d2d::IShape* loadChain(const Json::Value& value);
	static d2d::IShape* loadRect(const Json::Value& value);
	static d2d::IShape* loadCircle(const Json::Value& value);

	static Json::Value store(const BezierShape* bezier);
	static Json::Value store(const PolygonShape* poly);
	static Json::Value store(const ChainShape* chain);
	static Json::Value store(const RectShape* rect);
	static Json::Value store(const CircleShape* circle);

public:
	std::vector<d2d::IShape*>& m_shapes;

}; // FileAdapter

}

#endif // _LIBSHAPE_FILE_ADAPTER_H_