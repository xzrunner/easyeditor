#include "FileAdapter.h"

#include "BezierShape.h"
#include "ChainShape.h"
#include "CircleShape.h"
#include "CosineShape.h"
#include "PolygonShape.h"
#include "RectShape.h"

namespace libshape
{

FileAdapter::FileAdapter(std::vector<d2d::IShape*>& shapes)
	: m_shapes(shapes)
{
}

void FileAdapter::load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int i = 0;
	Json::Value shapeValue = value["shapes"][i++];
	while (!shapeValue.isNull()) {
		d2d::IShape* shape = loadShape(shapeValue);
		m_shapes.push_back(shape);
		shapeValue = value["shapes"][i++];
	}
}

void FileAdapter::store(const char* filename)
{
	Json::Value value;

	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
		value["shapes"][i] = store(m_shapes[i]);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

d2d::IShape* FileAdapter::loadShape(const Json::Value& value)
{
	d2d::IShape* shape = NULL;

	if (value.isNull())
		;
	else if (!value["bezier"].isNull())
		shape = loadBezier(value["bezier"]);
	else if (!value["polygon"].isNull())
		shape = loadPolygon(value["polygon"]);
	else if (!value["chain"].isNull())
		shape = loadChain(value["chain"]);
	else if (!value["rect"].isNull())
		shape = loadRect(value["rect"]);
	else if (!value["circle"].isNull())
		shape = loadCircle(value["circle"]);

	return shape;
}

Json::Value FileAdapter::store(d2d::IShape* shape)
{
	Json::Value value;

	if (BezierShape* bezier = dynamic_cast<BezierShape*>(shape))
		value["bezier"] = store(bezier);
	else if (PolygonShape* poly = dynamic_cast<PolygonShape*>(shape))
		value["polygon"] = store(poly);
	else if (ChainShape* chain = dynamic_cast<ChainShape*>(shape))
		value["chain"] = store(chain);
	else if (RectShape* rect = dynamic_cast<RectShape*>(shape))
		value["rect"] = store(rect);
	else if (CircleShape* circle = dynamic_cast<CircleShape*>(shape))
		value["circle"] = store(circle);

	return value;
}

d2d::IShape* FileAdapter::loadBezier(const Json::Value& value)
{
	d2d::Vector points[4];
	for (size_t i = 0; i < 4; ++i)
	{
		points[i].x = value["points"]["x"][i].asDouble();
		points[i].y = value["points"]["y"][i].asDouble();
	}

	BezierShape* bezier = new BezierShape(points);
	bezier->name = value["name"].asString();

	return bezier;
}

d2d::IShape* FileAdapter::loadPolygon(const Json::Value& value)
{
	std::vector<d2d::Vector> vertices;
	size_t num = value["vertices"]["x"].size();
	vertices.resize(num);
	for (size_t i = 0; i < num; ++i)
	{
		vertices[i].x = value["vertices"]["x"][i].asDouble();
		vertices[i].y = value["vertices"]["y"][i].asDouble();
	}

	PolygonShape* poly = new PolygonShape(vertices);
	poly->name = value["name"].asString();

	return poly;
}

d2d::IShape* FileAdapter::loadChain(const Json::Value& value)
{
	std::vector<d2d::Vector> vertices;
	size_t num = value["vertices"]["x"].size();
	vertices.resize(num);
	for (size_t i = 0; i < num; ++i)
	{
		vertices[i].x = value["vertices"]["x"][i].asDouble();
		vertices[i].y = value["vertices"]["y"][i].asDouble();
	}

	bool isLoop = value["closed"].asBool();

	ChainShape* chain = new ChainShape(vertices, isLoop);
	chain->name = value["name"].asString();

	return chain;
}

d2d::IShape* FileAdapter::loadRect(const Json::Value& value)
{
	const float xmin = value["xmin"].asDouble(),
		xmax = value["xmax"].asDouble(),
		ymin = value["ymin"].asDouble(),
		ymax = value["ymax"].asDouble();

	RectShape* rect = new RectShape(d2d::Vector(xmin, ymin), d2d::Vector(xmax, ymax));
	rect->name = value["name"].asString();

	return rect;
}

d2d::IShape* FileAdapter::loadCircle(const Json::Value& value)
{
	const float x = value["x"].asDouble(),
		y = value["y"].asDouble(),
		radius = value["radius"].asDouble();

	CircleShape* circle = new CircleShape(d2d::Vector(x, y), radius);
	circle->name = value["name"].asString();

	return circle;
}

Json::Value FileAdapter::store(const BezierShape* bezier)
{
	Json::Value value;

	value["name"] = bezier->name;

	for (size_t i = 0; i < 4; ++i)
	{
		value["points"]["x"][i] = bezier->points[i].x;
		value["points"]["y"][i] = bezier->points[i].y;
	}

	return value;
}

Json::Value FileAdapter::store(const PolygonShape* poly)
{
	Json::Value value;

	value["name"] = poly->name;

	const std::vector<d2d::Vector>& vertices = poly->getVertices();
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		value["vertices"]["x"][i] = vertices[i].x;
		value["vertices"]["y"][i] = vertices[i].y;
	}

	return value;
}

Json::Value FileAdapter::store(const ChainShape* chain)
{
	Json::Value value;

	value["name"] = chain->name;

	const std::vector<d2d::Vector>& vertices = chain->getVertices();
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		value["vertices"]["x"][i] = vertices[i].x;
		value["vertices"]["y"][i] = vertices[i].y;
	}
	value["closed"] = chain->isClosed();

	return value;
}

Json::Value FileAdapter::store(const RectShape* rect)
{
	Json::Value value;

	value["name"] = rect->name;

	value["xmin"] = rect->m_rect.xMin;
	value["xmax"] = rect->m_rect.xMax;
	value["ymin"] = rect->m_rect.yMin;
	value["ymax"] = rect->m_rect.yMax;

	return value;
}

Json::Value FileAdapter::store(const CircleShape* circle)
{
	Json::Value value;

	value["name"] = circle->name;

	value["x"] = circle->center.x;
	value["y"] = circle->center.y;
	value["radius"] = circle->radius;

	return value;
}

}