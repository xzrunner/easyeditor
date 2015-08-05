#include "SeparateToComplex.h"
#include "check_params.h"

#include <easyshape.h>
#include <wx/stdpaths.h>

namespace edb
{

std::string SeparateToComplex::Command() const
{
	return "separate2complex";
}

std::string SeparateToComplex::Description() const
{
	return "separate lr's sprites to easycompex file";
}

std::string SeparateToComplex::Usage() const
{
	// separate2complex e:/test2/test_lr.json point _output
	std::string usage = Command() + " [filepath] [point dir] [tmp dir]";
	return usage;
}

void SeparateToComplex::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return;
	if (!check_file(argv[2])) return;

	m_point_dir = argv[3];
	m_output_dir = d2d::FilenameTools::getFileDir(argv[4]);
	m_output_name = d2d::FilenameTools::getFilenameWithExtension(argv[4]);

	Run(argv[2]);
}

void SeparateToComplex::Run(const std::string& lr_file, const std::string& point_dir, 
							const std::string& dst_file)
{
	m_point_dir = point_dir;	
	m_output_dir = d2d::FilenameTools::getFileDir(dst_file);
	m_output_name = d2d::FilenameTools::getFilenameWithExtension(dst_file);

	Run(lr_file);
}

void SeparateToComplex::Run(const std::string& filepath)
{
	Json::Value lr_val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, lr_val);
	fin.close();

	Json::Value new_lr_val = lr_val;

	m_dir = d2d::FilenameTools::getFileDir(filepath);

	std::string dst_folder = m_output_dir;
	d2d::mk_dir(dst_folder, false);

	for (int layer_idx = 0; layer_idx < 8; ++layer_idx)
	{
		if (layer_idx == 1)
		{
			int idx = 0;
			Json::Value src_val = lr_val["layer"][layer_idx]["sprite"][idx++];
			while (!src_val.isNull()) {
				Json::Value& dst_val = 	new_lr_val["layer"][layer_idx]["sprite"][idx-1];
				SeparateSprite(src_val, dst_val);
				src_val = lr_val["layer"][layer_idx]["sprite"][idx++];
			}
		}
		else if (layer_idx == 0 || layer_idx == 2 || layer_idx == 3 || layer_idx == 7)
		{
			int idx = 0;
			Json::Value src_val = lr_val["layer"][layer_idx]["sprite"][idx++];
			while (!src_val.isNull()) {
				Json::Value& dst_val = 	new_lr_val["layer"][layer_idx]["sprite"][idx-1];
				FixSpriteName(src_val, dst_val);
				src_val = lr_val["layer"][layer_idx]["sprite"][idx++];
			}
		}
	}

	std::string outfile = dst_folder + "\\" + m_output_name;

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(outfile.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, new_lr_val);
	fout.close();
}

void SeparateToComplex::SeparateSprite(const Json::Value& src, Json::Value& dst)
{
	std::string name = CreateNewComplexFile(src);
	ResetOldSpriteVal(dst, name);
}

void SeparateToComplex::FixSpriteName(const Json::Value& src, Json::Value& dst)
{
	wxString relative_path = d2d::FilenameTools::getRelativePath(m_output_dir, 
		m_dir + "\\" + dst["filepath"].asString());
	dst["filepath"] = relative_path.ToStdString();
}

std::string SeparateToComplex::CreateNewComplexFile(const Json::Value& value) const
{
	std::string name = wxString::Format("lr_decorate_%d", m_count++).ToStdString();

	Json::Value out_val;

	out_val["name"] = name;
	out_val["use_render_cache"] = false;
	out_val["xmin"] = 0;
	out_val["xmax"] = 0;
	out_val["ymin"] = 0;
	out_val["ymax"] = 0;

	Json::Value spr_val = value;

	wxString relative_path = d2d::FilenameTools::getRelativePath(m_output_dir, 
		m_dir + "\\" + spr_val["filepath"].asString());
	spr_val["filepath"] = relative_path.ToStdString();

	d2d::Vector pos;
	pos.x = spr_val["position"]["x"].asDouble();
	pos.y = spr_val["position"]["y"].asDouble();
	FixPosWithShape(pos, value["filepath"].asString());

	spr_val["position"]["x"] = pos.x;
	spr_val["position"]["y"] = pos.y;

	int idx = 0;
	out_val["sprite"][idx] = spr_val;

	std::string outpath = m_output_dir + "\\" + name + "_complex.json";
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(outpath.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, out_val);
	fout.close();

	return name;
}

void SeparateToComplex::ResetOldSpriteVal(Json::Value& val, const std::string& name) const
{
	val["filepath"] = name + "_complex.json";
	val["position"]["x"] = 0;
	val["position"]["y"] = 0;

	val["angle"] = 0;
	val["x scale"] = 1;
	val["y scale"] = 1;
	val["x shear"] = 0;
	val["y shear"] = 0;
	val["x offset"] = 0;
	val["y offset"] = 0;
	val["x mirror"] = false;
	val["y mirror"] = false;

	val["name"] = name;
	val["tag"] = "";
	val["clip"] = false;

	val["multi color"] = "0xffffffff";
	val["add color"] = "0x00000000";
	val["r trans"] = "0xff0000ff";
	val["g trans"] = "0x00ff00ff";
	val["b trans"] = "0x0000ffff";
}

void SeparateToComplex::FixPosWithShape(d2d::Vector& pos, const std::string& filepath) const
{
	std::string path = filepath.substr(0, filepath.find_last_of('.')) + "_shape.json";
	std::string shape_path = m_dir + "\\" + m_point_dir + "\\" + path;
	if (!d2d::FilenameTools::isExist(shape_path)) {
		return;
	}

	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(shape_path);
	libshape::Symbol* shape_symbol = dynamic_cast<libshape::Symbol*>(symbol);
	if (!shape_symbol) {
		throw d2d::Exception("shape file err:%s", filepath);
	}
	
	std::vector<d2d::IShape*> shapes = shape_symbol->GetShapes();
	if (shapes.empty()) {
		throw d2d::Exception("shape file empty:%s", filepath);
	}

	if (libshape::PointShape* point = dynamic_cast<libshape::PointShape*>(shapes[0])) {
		pos += point->GetPos();
	} else {
		throw d2d::Exception("shape file is not point:%s", filepath);
	}

	symbol->Release();
}

}