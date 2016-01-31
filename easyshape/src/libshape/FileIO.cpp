#include "FileIO.h"
#include "ShapeType.h"
#include "ShapeFactory.h"

#include "BezierShape.h"
#include "ChainShape.h"
#include "CircleShape.h"
#include "CosineShape.h"
#include "PolygonShape.h"
#include "RectShape.h"

namespace libshape
{

void FileIO::LoadFromFile(const char* filename, 
						  std::vector<ee::Shape*>& shapes, 
						  std::string& bg_filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filename);
	bg_filepath = dir + "\\" + value["bg_symbol"].asString();

	int i = 0;
	Json::Value shapeValue = value["shapes"][i++];
	while (!shapeValue.isNull()) {
		ee::Shape* shape = LoadShape(dir, shapeValue);
		shapes.push_back(shape);
		shapeValue = value["shapes"][i++];
	}
}

void FileIO::LoadFromFile(const char* filename, 
						  std::vector<ee::Shape*>& shapes, 
						  ee::Symbol*& bg)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filename);

	int i = 0;
	Json::Value shapeValue = value["shapes"][i++];
	while (!shapeValue.isNull()) {
		ee::Shape* shape = LoadShape(dir, shapeValue);
		shapes.push_back(shape);
		shapeValue = value["shapes"][i++];
	}

	if (!value["bg_symbol"].isNull()) {
		std::string path = ee::FileHelper::GetAbsolutePath(dir, value["bg_symbol"].asString());
		ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(path);
		ee::obj_assign<ee::Symbol>(bg, symbol);
		symbol->Release();
	}
}

void FileIO::StoreToFile(const char* filename, 
						 const std::vector<ee::Shape*>& shapes, 
						 const ee::Symbol* bg)
{
	std::string dir = ee::FileHelper::GetFileDir(filename);
	Json::Value value;
	for (size_t i = 0, n = shapes.size(); i < n; ++i) {
		value["shapes"][i] = StoreShape(dir, shapes[i]);
	}

	if (bg) {
		wxString dir = ee::FileHelper::GetFileDir(filename) + "\\";
		value["bg_symbol"] = ee::FileHelper::GetRelativePath(dir,
			bg->GetFilepath()).ToStdString();
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

ee::Shape* FileIO::LoadShape(const std::string& dir, const Json::Value& value)
{
	ee::Shape* shape = NULL;

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

Json::Value FileIO::StoreShape(const std::string& dir, ee::Shape* shape)
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
	shape->StoreToFile(value, dir);

	return value;
}

ee::Shape* FileIO::LoadBezier(const Json::Value& value)
{
	BezierShape* bezier = new BezierShape();
	bezier->LoadFromFile(value, "");
	return bezier;
}

ee::Shape* FileIO::LoadPolygon(const std::string& dir, const Json::Value& value)
{
	PolygonShape* poly = new PolygonShape;
	poly->LoadFromFile(value, dir);
	return poly;
}

ee::Shape* FileIO::LoadChain(const Json::Value& value)
{
	ChainShape* chain = new ChainShape();
	chain->LoadFromFile(value, "");
	return chain;
}

ee::Shape* FileIO::LoadRect(const Json::Value& value)
{
	RectShape* rect = new RectShape();
	rect->LoadFromFile(value, "");
	return rect;
}

ee::Shape* FileIO::LoadCircle(const Json::Value& value)
{
	CircleShape* circle = new CircleShape();
	circle->LoadFromFile(value, "");
	return circle;
}

}