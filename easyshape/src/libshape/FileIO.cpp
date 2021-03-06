#include "FileIO.h"
#include "ShapeType.h"
#include "ShapeFactory.h"

#include "BezierShape.h"
#include "ChainShape.h"
#include "CircleShape.h"
#include "CosineShape.h"
#include "PolygonShape.h"
#include "RectShape.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/Symbol.h>

#include <fstream>

namespace eshape
{

ee::ShapePtr FileIO::LoadFromFile(const char* filename, std::string& bg_filepath)
{
	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filename);
	bg_filepath = dir + "\\" + val["bg_symbol"].asString();

	return LoadShape(dir, val["shape"]);
}

ee::ShapePtr FileIO::LoadFromFile(const char* filename, ee::SymPtr& bg)
{
	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filename);

	if (!val["bg_symbol"].isNull()) {
		std::string path = ee::FileHelper::GetAbsolutePath(dir, val["bg_symbol"].asString());
		bg = ee::SymbolMgr::Instance()->FetchSymbol(path);		
	}

	return LoadShape(dir, val["shape"]);
}

void FileIO::StoreToFile(const char* filename, 
						 const ee::Shape& shape,
						 const ee::SymPtr& bg)
{
	std::string dir = ee::FileHelper::GetFileDir(filename);
	Json::Value val;
	
	val["shape"] = StoreShape(dir, shape);

	if (bg) {
		std::string dir = ee::FileHelper::GetFileDir(filename) + "\\";
		val["bg_symbol"] = ee::FileHelper::GetRelativePath(dir, bg->GetFilepath());
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, val);
	fout.close();
}

ee::ShapePtr FileIO::LoadShape(const std::string& dir, const Json::Value& value)
{
	ee::ShapePtr shape = nullptr;

// 	// old
// 	if (value.isNull())
// 		;
// 	else if (!value["bezier"].isNull())
// 		shape = LoadBezier(value["bezier"]);
// 	else if (!value["polygon"].isNull())
// 		shape = LoadPolygon(dir, value["polygon"]);
// 	else if (!value["chain"].isNull())
// 		shape = LoadChain(value["chain"]);
// 	else if (!value["rect"].isNull())
// 		shape = LoadRect(value["rect"]);
// 	else if (!value["circle"].isNull())
// 		shape = LoadCircle(value["circle"]);

	// new
	shape = ShapeFactory::CreateShapeFromFile(value, dir);

	return shape;
}

Json::Value FileIO::StoreShape(const std::string& dir, const ee::Shape& shape)
{
	Json::Value value;

// 	// old
// 	std::string title;
// 
// 	ShapeType type = get_shape_type(shape->GetShapeDesc());
// 	switch (type)
// 	{
// 	case ST_BEZIER:
// 		title = "bezier";
// 		break;
// 	case ST_POLYGON:
// 		title = "polygon";
// 		break;
// 	case ST_CHAIN:
// 		title = "chain";
// 		break;
// 	case ST_RECT:
// 		title = "rect";
// 		break;
// 	case ST_CIRCLE:
// 		title = "circle";
// 		break;
// 	}
// 
// 	shape->StoreToFile(value[title], dir);

	// new
	shape.StoreToFile(value, dir);

	return value;
}

ee::ShapePtr FileIO::LoadBezier(const Json::Value& value)
{
	auto bezier = std::make_unique<BezierShape>();
	bezier->LoadFromFile(value, "");
	return bezier;
}

ee::ShapePtr FileIO::LoadPolygon(const std::string& dir, const Json::Value& value)
{
	auto poly = std::make_unique<PolygonShape>();
	poly->LoadFromFile(value, dir.c_str());
	return poly;
}

ee::ShapePtr FileIO::LoadChain(const Json::Value& value)
{
	auto chain = std::make_unique<ChainShape>();
	chain->LoadFromFile(value, "");
	return chain;
}

ee::ShapePtr FileIO::LoadRect(const Json::Value& value)
{
	auto rect = std::make_unique<RectShape>();
	rect->LoadFromFile(value, "");
	return rect;
}

ee::ShapePtr FileIO::LoadCircle(const Json::Value& value)
{
	auto circle = std::make_unique<CircleShape>();
	circle->LoadFromFile(value, "");
	return circle;
}

}