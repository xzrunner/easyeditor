#include "LRToShapeTable.h"
#include "check_params.h"

#include <lr/dataset/Grids.h>

#include <easyshape.h>

namespace edb
{

std::string LRToShapeTable::Command() const
{
	return "lr2shape";
}

std::string LRToShapeTable::Description() const
{
	return "create shape table from lr file";
}

std::string LRToShapeTable::Usage() const
{
	// lr2shape e:/test2/test_lr.json
	std::string usage = Command() + " [filepath]";
	return usage;
}

void LRToShapeTable::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_file(argv[2])) return;

	Run(argv[2]);
}

void LRToShapeTable::Run(const std::string& filepath)
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

	// collision region
	int idx = 0;
	Json::Value spr_val = lr_val["layer"][(int)6]["sprite"][idx++];
	while (!spr_val.isNull()) {
		wxString spr_path = d2d::SymbolSearcher::GetSymbolPath(dir, spr_val);
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(spr_path);
		assert(symbol);

		Json::Value shape_val;
		shape_val["name"] = spr_val["name"];

		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->load(spr_val);

		libshape::Sprite* shape_spr = dynamic_cast<libshape::Sprite*>(sprite);
		assert(shape_spr);
		const std::vector<d2d::IShape*>& shapes = shape_spr->getSymbol().GetShapes();
		for (int i = 0, n = shapes.size(); i < n; ++i)
		{
			d2d::IShape* shape = shapes[i];
			libshape::PolygonShape* poly = dynamic_cast<libshape::PolygonShape*>(shape);
			if (!poly) {
				continue;
			}
			const std::vector<d2d::Vector>& bound = poly->GetVertices();
			std::vector<int> grid_idx = grids.IntersectPolygon(bound);
			for (int i = 0, n = grid_idx.size(); i < n; ++i) {
				int sz = shape_val["grid"].size();
				shape_val["grid"][sz] = grid_idx[i];
			}
		}

		int sz = out_val["collision region"].size();
		out_val["collision region"][sz] = shape_val;

		sprite->Release();
		symbol->Release();

		spr_val = lr_val["layer"][(int)6]["sprite"][idx++];
	}

	std::string outfile = filepath.substr(0, filepath.find_last_of(".")) + "_shape.json";

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(outfile.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, out_val);
	fout.close();
}

}