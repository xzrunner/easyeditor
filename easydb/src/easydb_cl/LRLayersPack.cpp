#include "LRLayersPack.h"
#include "check_params.h"

#include <lr/dataset/Grids.h>
#include <lr/dataset/CharacterFileName.h>

#include <easyshape.h>

namespace edb
{

std::string LRLayersPack::Command() const
{
	return "lr-pack";
}

std::string LRLayersPack::Description() const
{
	return "create shape table from lr file";
}

std::string LRLayersPack::Usage() const
{
	// lr-pack e:/test2/test_lr.json
	std::string usage = Command() + " [filepath]";
	return usage;
}

void LRLayersPack::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_file(argv[2])) return;

	Run(argv[2]);
}

void LRLayersPack::Run(const std::string& filepath)
{
	Json::Value lr_val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, lr_val);
	fin.close();

	std::string dir = d2d::FilenameTools::getFileDir(filepath) + "\\";

	Json::Value out_val;

	out_val["width"] = lr_val["size"]["width"];
	out_val["height"] = lr_val["size"]["height"];

	lr::Grids grids;
	int w = lr_val["size"]["width"].asUInt(),
		h = lr_val["size"]["height"].asUInt();
	grids.Build(w, h);

	int col, row;
	grids.GetGridSize(col, row);
	out_val["col"] = col;
	out_val["row"] = row;

	ParserCharacter(lr_val, 2, "character", out_val);
	ParserPoint(lr_val, dir, 3, "point", out_val);
	ParserPolygon(lr_val, dir, grids, 4, "path", out_val);
	ParserPolygon(lr_val, dir, grids, 5, "region", out_val);
	ParserPolygon(lr_val, dir, grids, 6, "collision region", out_val);
	ParserCamera(lr_val, 7, "camera", out_val);

	std::string outfile = filepath.substr(0, filepath.find_last_of('_')) + ".json";

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(outfile.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, out_val);
	fout.close();
}

void LRLayersPack::ParserPolyShape(d2d::IShape* shape, const d2d::Vector& offset, 
								   const lr::Grids& grids, Json::Value& out_val)
{
	std::vector<int> grid_idx;
	if (libshape::PolygonShape* poly = dynamic_cast<libshape::PolygonShape*>(shape))
	{
		std::vector<d2d::Vector> bound = poly->GetVertices();
		for (int i = 0, n = bound.size(); i < n; ++i) {
			bound[i] += offset;
		}
		grid_idx = grids.IntersectPolygon(bound);
	}
	else if (libshape::ChainShape* chain = dynamic_cast<libshape::ChainShape*>(shape))
	{
		std::vector<d2d::Vector> bound = chain->GetVertices();
		for (int i = 0, n = bound.size(); i < n; ++i) {
			bound[i] += offset;
		}
		grid_idx = grids.IntersectPolyline(bound);
	}
	else
	{
		throw d2d::Exception("LRLayersPack::ParserPolyLayer error shape type");
	}

	for (int i = 0, n = grid_idx.size(); i < n; ++i) {
		int sz = out_val["grid"].size();
		out_val["grid"][sz] = grid_idx[i];
	}
}

void LRLayersPack::ParserPolygon(const Json::Value& src_val, const std::string& dir,
								 const lr::Grids& grids, int layer_idx, const char* name, Json::Value& out_val)
{
	int idx = 0;
	Json::Value src_spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
	while (!src_spr_val.isNull()) 
	{
		wxString spr_path = d2d::SymbolSearcher::GetSymbolPath(dir, src_spr_val);
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(spr_path);
		assert(symbol);

		Json::Value dst_val;
		dst_val["name"] = src_spr_val["name"];

		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->load(src_spr_val);

		libshape::Sprite* shape_spr = dynamic_cast<libshape::Sprite*>(sprite);
		assert(shape_spr);
		const std::vector<d2d::IShape*>& shapes = shape_spr->getSymbol().GetShapes();
		for (int i = 0, n = shapes.size(); i < n; ++i) {
			ParserPolyShape(shapes[i], sprite->getPosition(), grids, dst_val);
		}

		int sz = out_val[name].size();
		out_val[name][sz] = dst_val;

		sprite->Release();
		symbol->Release();

		src_spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
	}

	idx = 0;
	Json::Value src_shape_val = src_val["layer"][layer_idx]["shape"][idx++];
	while (!src_shape_val.isNull()) 
	{
		d2d::IShape* shape = libshape::ShapeFactory::CreateShapeFromFile(src_shape_val, dir);
		
		Json::Value dst_val;
		dst_val["name"] = src_shape_val["name"];
		ParserPolyShape(shape, d2d::Vector(0, 0), grids, dst_val);

		int sz = out_val[name].size();
		out_val[name][sz] = dst_val;

		shape->Release();

		src_shape_val = src_val["layer"][layer_idx]["shape"][idx++];
	}
}

void LRLayersPack::ParserPoint(const Json::Value& src_val, const std::string& dir,
									int layer_idx, const char* name, Json::Value& out_val)
{
	int idx = 0;
	Json::Value spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
	while (!spr_val.isNull()) 
	{
		wxString spr_path = d2d::SymbolSearcher::GetSymbolPath(dir, spr_val);
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(spr_path);
		assert(symbol);

		Json::Value shape_val;
		shape_val["name"] = spr_val["name"];

		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->load(spr_val);

		shape_val["x"] = sprite->getPosition().x;
		shape_val["y"] = sprite->getPosition().y;

		int sz = out_val[name].size();
		out_val[name][sz] = shape_val;

		sprite->Release();
		symbol->Release();

		spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
	}
}

void LRLayersPack::ParserCamera(const Json::Value& src_val, int layer_idx, 
									 const char* name, Json::Value& out_val)
{
	int idx = 0;
	Json::Value spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
	while (!spr_val.isNull()) 
	{
		Json::Value cam_val;
		cam_val["name"] = spr_val["name"];
		cam_val["x"] = spr_val["position"]["x"];
		cam_val["y"] = spr_val["position"]["y"];
		cam_val["scale"] = spr_val["x scale"];

		int sz = out_val[name].size();
		out_val[name][sz] = cam_val;

		spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
	}
}

void LRLayersPack::ParserCharacter(const Json::Value& src_val, int layer_idx, 
								   const char* name, Json::Value& out_val)
{
	int idx = 0;
	Json::Value spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
	while (!spr_val.isNull()) 
	{
		Json::Value char_val;
		char_val["name"] = spr_val["name"];
		char_val["x"] = spr_val["position"]["x"];
		char_val["y"] = spr_val["position"]["y"];

		// tags
		std::string tag = spr_val["tag"].asString();
		std::vector<std::string> tags;
		int pos = tag.find_first_of(';');
		tags.push_back(tag.substr(0, pos));
		do 
		{
			int next_pos = tag.find_first_of(';', pos + 1);
			tags.push_back(tag.substr(pos + 1, next_pos - pos - 1));
			pos = next_pos;
		} while (pos != std::string::npos);
		for (int i = 0, n = tags.size(); i < n; ++i) {
			const std::string& str = tags[i];
			int pos = str.find_first_of('=');
			std::string key = str.substr(0, pos);
			std::string val = str.substr(pos+1);
			char_val["tag"][key] = val;
		}

		// filename
		std::string filename = d2d::FilenameTools::getFilename(spr_val["filepath"].asString());
		lr::CharacterFileName out_name(filename);
		char_val["filename"] = out_name.GetOutputName();

		// angle
		int dir = 1 + (out_name.GetField(lr::CharacterFileName::FT_DIRECTION)[0] - '1');
		if (spr_val["x mirror"].asBool()) {
			dir = 10 - dir;
		}
		dir = (dir + 7) % 8;
		char_val["angle"] = dir + 1;

		int sz = out_val[name].size();
		out_val[name][sz] = char_val;

		spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
	}
}

}