#include "LRSeparateComplex.h"
#include "check_params.h"
#include "lr_typedef.h"

#include <ee/FileHelper.h>
#include <ee/StringHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/SpriteIO.h>
#include <ee/SymbolFile.h>

#include <easyshape.h>

#include <sprite2/SymType.h>

#include <wx/stdpaths.h>

namespace edb
{

std::string LRSeparateComplex::Command() const
{
	return "separate2complex";
}

std::string LRSeparateComplex::Description() const
{
	return "separate lr's sprs to easycompex file";
}

std::string LRSeparateComplex::Usage() const
{
	// separate2complex e:/test2/test_lr.json point _output
	std::string usage = Command() + " [filepath] [point dir] [tmp dir]";
	return usage;
}

int LRSeparateComplex::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (!check_file(argv[2])) return -1;

	m_point_dir = argv[3];
	m_output_dir = ee::FileHelper::GetFileDir(argv[4]);
	m_output_name = ee::FileHelper::GetFilenameWithExtension(argv[4]);

	Run(argv[2]);

	return 0;
}

void LRSeparateComplex::Run(const std::string& lr_file, const std::string& point_dir, 
							const std::string& dst_file)
{
	m_point_dir = point_dir;	
	m_output_dir = ee::FileHelper::GetFileDir(dst_file);
	m_output_name = ee::FileHelper::GetFilenameWithExtension(dst_file);

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

	m_dir = ee::FileHelper::GetFileDir(filepath);

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
// 		if (!is_cover &&
// 			ee::SymbolFile::Instance()->Type(filepath) == s2::SYM_PARTICLE3D &&
// 			tag.find(TOP_LAYER_STR) == std::string::npos) {
// 				src_val["tag"] = tag + ";" + COVER_LAYER_STR;
// 				is_cover = true;
// 		} 

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
	std::string relative_path = ee::FileHelper::GetRelativePath(m_output_dir, 
		m_dir + "\\" + dst["filepath"].asString());
	dst["filepath"] = relative_path;
}

std::string LRSeparateComplex::CreateNewComplexFile(const Json::Value& value) const
{
	std::string name = std::string("name_") + ee::StringHelper::ToString(m_count++);

	Json::Value out_val;

	out_val["name"] = name;
	out_val["use_render_cache"] = false;
	out_val["xmin"] = 0;
	out_val["xmax"] = 0;
	out_val["ymin"] = 0;
	out_val["ymax"] = 0;

	Json::Value spr_val = value;

	std::string relative_path = ee::FileHelper::GetRelativePath(m_output_dir, 
		m_dir + "\\" + spr_val["filepath"].asString());
	spr_val["filepath"] = relative_path;

	std::string dir = ee::FileHelper::GetFileDir(relative_path);

	ee::SpriteIO spr_io;
	spr_io.Load(spr_val, dir);
	FixPosWithShape(spr_io.m_position, value["filepath"].asString());
	int idx = 0;
	out_val["sprite"][idx] = spr_val;
	spr_io.Store(out_val["sprite"][idx], dir);

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

	Json::Value _val;
	_val["filepath"] = export_name + "_complex.json";
	_val["export"] = export_name;

	ee::SpriteIO spr_io;
	spr_io.m_tag = tag;
	spr_io.Store(_val, "");

	val = _val;
}

void LRSeparateComplex::FixPosWithShape(sm::vec2& pos, const std::string& filepath) const
{
	std::string path = filepath.substr(0, filepath.find_last_of('.')) + "_shape.json";
	std::string shape_path = m_dir + "\\" + m_point_dir + "\\" + path;
	if (!ee::FileHelper::IsFileExist(shape_path)) {
		return;
	}

	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(shape_path);
	eshape::Symbol* shape_symbol = dynamic_cast<eshape::Symbol*>(sym);
	if (!shape_symbol) {
		throw ee::Exception("shape file err:%s", filepath);
	}
	
	const s2::Shape* shape = shape_symbol->GetShape();
	if (!shape) {
		throw ee::Exception("shape file empty:%s", filepath);
	}

	if (const eshape::PointShape* point = dynamic_cast<const eshape::PointShape*>(shape)) {
		pos += point->GetPos();
	} else {
		throw ee::Exception("shape file is not point:%s", filepath);
	}

	sym->RemoveReference();
}

}