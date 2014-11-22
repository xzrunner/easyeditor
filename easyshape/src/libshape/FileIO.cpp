#include "FileIO.h"

#include "BezierShape.h"
#include "ChainShape.h"
#include "CircleShape.h"
#include "CosineShape.h"
#include "PolygonShape.h"
#include "RectShape.h"

namespace libshape
{

void FileIO::LoadFromFile(const char* filename, 
						  std::vector<d2d::IShape*>& shapes, 
						  d2d::ISymbol*& bg)
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
		d2d::IShape* shape = LoadShape(shapeValue);
		shapes.push_back(shape);
		shapeValue = value["shapes"][i++];
	}

	wxString dir = d2d::FilenameTools::getFileDir(filename) + "\\";
	wxString path = d2d::FilenameTools::getAbsolutePath(dir, value["bg_symbol"].asString());
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(path);
	d2d::obj_assign((d2d::Object*&)bg, symbol);
	symbol->Release();
}

void FileIO::StoreToFile(const char* filename, 
						 const std::vector<d2d::IShape*>& shapes, 
						 const d2d::ISymbol* bg)
{
	Json::Value value;

	for (size_t i = 0, n = shapes.size(); i < n; ++i) {
		value["shapes"][i] = StoreShape(shapes[i]);
	}

	if (bg) {
		wxString dir = d2d::FilenameTools::getFileDir(filename) + "\\";
		value["bg_symbol"] = d2d::FilenameTools::getRelativePath(dir,
			bg->getFilepath()).ToStdString();
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

d2d::IShape* FileIO::LoadShape(const Json::Value& value)
{
	d2d::IShape* shape = NULL;

	if (value.isNull())
		;
	else if (!value["bezier"].isNull())
		shape = LoadBezier(value["bezier"]);
	else if (!value["polygon"].isNull())
		shape = LoadPolygon(value["polygon"]);
	else if (!value["chain"].isNull())
		shape = LoadChain(value["chain"]);
	else if (!value["rect"].isNull())
		shape = LoadRect(value["rect"]);
	else if (!value["circle"].isNull())
		shape = LoadCircle(value["circle"]);

	return shape;
}

Json::Value FileIO::StoreShape(d2d::IShape* shape)
{
	Json::Value value;

	if (BezierShape* bezier = dynamic_cast<BezierShape*>(shape)) {
		value["bezier"] = StoreBezier(bezier);
	} else if (PolygonShape* poly = dynamic_cast<PolygonShape*>(shape)) {
		value["polygon"] = StorePolygon(poly);
	} else if (ChainShape* chain = dynamic_cast<ChainShape*>(shape)) {
		value["chain"] = StoreChain(chain);
	} else if (RectShape* rect = dynamic_cast<RectShape*>(shape)) {
		value["rect"] = StoreRect(rect);
	} else if (CircleShape* circle = dynamic_cast<CircleShape*>(shape)) {
		value["circle"] = StoreCircle(circle);
	}

	return value;
}

d2d::IShape* FileIO::LoadBezier(const Json::Value& value)
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

d2d::IShape* FileIO::LoadPolygon(const Json::Value& value)
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

d2d::IShape* FileIO::LoadChain(const Json::Value& value)
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

d2d::IShape* FileIO::LoadRect(const Json::Value& value)
{
	const float xmin = value["xmin"].asDouble(),
		xmax = value["xmax"].asDouble(),
		ymin = value["ymin"].asDouble(),
		ymax = value["ymax"].asDouble();

	RectShape* rect = new RectShape(d2d::Vector(xmin, ymin), d2d::Vector(xmax, ymax));
	rect->name = value["name"].asString();

	return rect;
}

d2d::IShape* FileIO::LoadCircle(const Json::Value& value)
{
	const float x = value["x"].asDouble(),
		y = value["y"].asDouble(),
		radius = value["radius"].asDouble();

	CircleShape* circle = new CircleShape(d2d::Vector(x, y), radius);
	circle->name = value["name"].asString();

	return circle;
}

Json::Value FileIO::StoreBezier(const BezierShape* bezier)
{
	Json::Value value;

	value["name"] = bezier->name;

	const d2d::Vector* ctrl_nodes = bezier->GetCtrlNode();
	for (int i = 0; i < BezierShape::CTRL_NODE_COUNT; ++i) {
		value["points"]["x"][i] = ctrl_nodes[i].x;
		value["points"]["y"][i] = ctrl_nodes[i].y;
	}

	return value;
}

Json::Value FileIO::StorePolygon(const PolygonShape* poly)
{
	Json::Value value;

	value["name"] = poly->name;

	const std::vector<d2d::Vector>& vertices = poly->GetVertices();
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		value["vertices"]["x"][i] = vertices[i].x;
		value["vertices"]["y"][i] = vertices[i].y;
	}

	return value;
}

Json::Value FileIO::StoreChain(const ChainShape* chain)
{
	Json::Value value;

	value["name"] = chain->name;

	const std::vector<d2d::Vector>& vertices = chain->GetVertices();
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		value["vertices"]["x"][i] = vertices[i].x;
		value["vertices"]["y"][i] = vertices[i].y;
	}
	value["closed"] = chain->IsClosed();

	return value;
}

Json::Value FileIO::StoreRect(const RectShape* rect)
{
	Json::Value value;

	value["name"] = rect->name;

	value["xmin"] = rect->m_rect.xMin;
	value["xmax"] = rect->m_rect.xMax;
	value["ymin"] = rect->m_rect.yMin;
	value["ymax"] = rect->m_rect.yMax;

	return value;
}

Json::Value FileIO::StoreCircle(const CircleShape* circle)
{
	Json::Value value;

	value["name"] = circle->name;

	value["x"] = circle->center.x;
	value["y"] = circle->center.y;
	value["radius"] = circle->radius;

	return value;
}

}