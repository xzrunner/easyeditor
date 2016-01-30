#include "LRSeparateComplex.h"
#include "check_params.h"
#include "lr_typedef.h"

#include <easyshape.h>
#include <wx/stdpaths.h>

namespace edb
{

std::string LRSeparateComplex::Command() const
{
	return "separate2complex";
}

std::string LRSeparateComplex::Description() const
{
	return "separate lr's sprites to easycompex file";
}

std::string LRSeparateComplex::Usage() const
{
	// separate2complex e:/test2/test_lr.json point _output
	std::string usage = Command() + " [filepath] [point dir] [tmp dir]";
	return usage;
}

void LRSeparateComplex::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return;
	if (!check_file(argv[2])) return;

	m_point_dir = argv[3];
	m_output_dir = d2d::FileHelper::GetFileDir(argv[4]);
	m_output_name = d2d::FileHelper::GetFilenameWithExtension(argv[4]);

	Run(argv[2]);
}

void LRSeparateComplex::Run(const std::string& lr_file, const std::string& point_dir, 
							const std::string& dst_file)
{
	m_point_dir = point_dir;	
	m_output_dir = d2d::FileHelper::GetFileDir(dst_file);
	m_output_name = d2d::FileHelper::GetFilenameWithExtension(dst_file);

	Run(lr_file);
}

void LRSeparateComplex::Run(const std::string& filepath)
{
	Json::Value lr_val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, lr_val);
	fin.close();

	Json::Value new_lr_val = lr_val;

	m_dir = d2d::FileHelper::GetFileDir(filepath);

	std::string dst_folder = m_output_dir;
	ee::FileHelper::MkDir(dst_folder, false);

	for (int layer_idx = 0; layer_idx < 9; ++layer_idx)
	{
		// shape layer
		if (layer_idx == 4 || layer_idx == 5 || layer_idx == 6) {
			continue;
		}

		const Json::Value& src_layer_val = lr_val["layer"][layer_idx];
		Json::Value& dst_layer_val = new_lr_val["layer"][layer_idx];

		SeparateFromSprites(src_layer_val, dst_layer_val);

		int idx = 0;
		Json::Value cl_val = src_layer_val["layers"][idx++];
		while (!cl_val.isNull()) {
			SeparateFromSprites(cl_val, dst_layer_val["layers"][idx - 1]);
			cl_val = src_layer_val["layers"][idx++];
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

void LRSeparateComplex::SeparateFromSprites(const Json::Value& old_val, Json::Value& new_val) 
{
	int idx = 0;
	Json::Value src_val = old_val["sprite"][idx++];
	while (!src_val.isNull()) {
		bool is_cover = false;

		std::string tag = src_val["tag"].asString();
		if (tag.find(COVER_LAYER_STR) != std::string::npos) {
			is_cover = true;
		}

		std::string filepath = src_val["filepath"].asString();
		if (!is_cover &&
			d2d::FileType::IsType(filepath, d2d::FileType::e_particle3d) &&
			tag.find(TOP_LAYER_STR) == std::string::npos) {
				src_val["tag"] = tag + ";" + COVER_LAYER_STR;
				is_cover = true;
		} 

		Json::Value& dst_val = new_val["sprite"][idx-1];
		if (is_cover) {
			SeparateSprite(src_val, dst_val);
		} else {
			FixSpriteName(src_val, dst_val);
		}

		src_val = old_val["sprite"][idx++];
	}
}

void LRSeparateComplex::SeparateSprite(const Json::Value& src, Json::Value& dst)
{
	std::string export_name = CreateNewComplexFile(src);
	ResetOldSpriteVal(dst, export_name, src["tag"].asString());
}

void LRSeparateComplex::FixSpriteName(const Json::Value& src, Json::Value& dst)
{
	wxString relative_path = d2d::FileHelper::GetRelativePath(m_output_dir, 
		m_dir + "\\" + dst["filepath"].asString());
	dst["filepath"] = relative_path.ToStdString();
}

std::string LRSeparateComplex::CreateNewComplexFile(const Json::Value& value) const
{
	std::string name = wxString::Format("name_%d", m_count++).ToStdString();

	Json::Value out_val;

	out_val["name"] = name;
	out_val["use_render_cache"] = false;
	out_val["xmin"] = 0;
	out_val["xmax"] = 0;
	out_val["ymin"] = 0;
	out_val["ymax"] = 0;

	Json::Value spr_val = value;

	wxString relative_path = d2d::FileHelper::GetRelativePath(m_output_dir, 
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

void LRSeparateComplex::ResetOldSpriteVal(Json::Value& val, const std::string& export_name, const std::string& tag) const
{
	val["filepath"] = export_name + "_complex.json";
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

	val["export"] = export_name;
	val["tag"] = tag;
	val["clip"] = false;

	val["multi color"] = "0xffffffff";
	val["add color"] = "0x00000000";
	val["r trans"] = "0xff0000ff";
	val["g trans"] = "0x00ff00ff";
	val["b trans"] = "0x0000ffff";
}

void LRSeparateComplex::FixPosWithShape(d2d::Vector& pos, const std::string& filepath) const
{
	std::string path = filepath.substr(0, filepath.find_last_of('.')) + "_shape.json";
	std::string shape_path = m_dir + "\\" + m_point_dir + "\\" + path;
	if (!d2d::FileHelper::IsFileExist(shape_path)) {
		return;
	}

	d2d::Symbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(shape_path);
	libshape::Symbol* shape_symbol = dynamic_cast<libshape::Symbol*>(symbol);
	if (!shape_symbol) {
		throw d2d::Exception("shape file err:%s", filepath);
	}
	
	std::vector<d2d::Shape*> shapes = shape_symbol->GetShapes();
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