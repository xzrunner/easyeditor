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
	out_val["grid edge"] = lr::Grids::EDGE;

	lr::Grids grids;
	int w = lr_val["size"]["width"].asUInt(),
		h = lr_val["size"]["height"].asUInt();
	grids.Build(w, h);

//	out_val["collision region"]

	// collision region
	int idx = 0;
	Json::Value spr_val = lr_val["layer"][(int)6]["sprite"][idx++];
	while (!spr_val.isNull()) {
		wxString spr_path = d2d::SymbolSearcher::GetSymbolPath(dir, spr_val);
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(spr_path);
		assert(symbol);

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
			std::vector<d2d::Vector> bound_flat;
			bound_flat.resize(bound.size());
			for (int i = 0, n = bound.size(); i < n; ++i) {
				bound_flat[i] = lr::Grids::TransToFlatView(bound[i]);
			}

			
		}

		sprite->Release();
		symbol->Release();

		spr_val = lr_val["layer"][(int)6]["sprite"][idx++];
	}
}

}