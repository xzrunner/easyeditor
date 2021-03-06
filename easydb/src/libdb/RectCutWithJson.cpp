#include "RectCutWithJson.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/ImageData.h>
#include <ee/StringHelper.h>
#include <ee/DummySprite.h>
#include <ee/DummySymbol.h>
#include <ee/DummySprite.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolType.h>
#include <ee/Exception.h>

#include <easyimage.h>
#include <easycomplex.h>
#include <easytexpacker.h>

#include <gimg_typedef.h>
#include <gimg_export.h>
#include <pimg/Condense.h>
#include <pimg/Cropping.h>
#include <pimg/Rect.h>
#include <sprite2/SymType.h>
#include <gum/Config.h>
#include <gum/FilepathHelper.h>
#include <gum/StringHelper.h>

namespace edb
{

static const char* IMAGE_DIR = "image";
static const char* JSON_DIR = "json";

RectCutWithJson::RectCutWithJson()
	: m_cfg(NULL)
{
}

RectCutWithJson::~RectCutWithJson()
{
	if (m_cfg) {
		delete m_cfg;
	}
}

std::string RectCutWithJson::Command() const
{
	return "rect-cut-with-json";
}

std::string RectCutWithJson::Description() const
{
	return "cut image by rect and fix json file";
}

std::string RectCutWithJson::Usage() const
{
	return Command() + " [src dir] [dst dir]";
}

int RectCutWithJson::Run(int argc, char *argv[])
{
	// rect-cut-with-json e:/test2/1001 e:/test2/1002

	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	if (argc > 4) 
	{
		if (strcmp(argv[4], "null") != 0) 
		{
			wxFileName filename(argv[4]);
			filename.Normalize();
			m_cfg = new Config(filename.GetFullPath().ToStdString());
		}
	}

	etexpacker::ImageTrimData* trim = NULL;
	if (argc > 5) {
		trim = new etexpacker::ImageTrimData(argv[5]);
	}

	gum::Config* cfg = gum::Config::Instance();
	bool old = cfg->GetPreMulAlpha();
	cfg->SetPreMulAlpha(false);
	Trigger(argv[2], argv[3], trim);
	cfg->SetPreMulAlpha(old);

	if (m_cfg) {
		m_cfg->Ouput();
	}

	return 0;
}

void RectCutWithJson::Trigger(const std::string& src_dir, const std::string& dst_dir,
							  const etexpacker::ImageTrimData* trim)
{
	std::string out_img_dir = dst_dir + "\\" + IMAGE_DIR;
	ee::FileHelper::MkDir(out_img_dir);
	std::string out_json_dir = dst_dir + "\\" + JSON_DIR;
	ee::FileHelper::MkDir(out_json_dir);

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		filepath = gum::FilepathHelper::Format(filepath.c_str()).c_str();
		if (m_cfg && m_cfg->IsIgnored(filepath)) {
			continue;
		}

		std::cout << i << " / " << n << " : " << filepath << "\n";
		int type = ee::SymbolFile::Instance()->Type(filepath);
		switch (type)
		{
		case s2::SYM_IMAGE:
			RectCutImage(src_dir, dst_dir, filepath, trim);
			break;
		case s2::SYM_COMPLEX:
			FixComplex(src_dir, dst_dir, filepath);
			break;
		case s2::SYM_ANIMATION:
			FixAnim(src_dir, dst_dir, filepath);
			break;
		case s2::SYM_SCALE9:
			FixScale9(src_dir, dst_dir, filepath);
			break;
		case s2::SYM_PARTICLE3D:
			FixParticle3d(src_dir, dst_dir, filepath);
			break;
		case s2::SYM_TRAIL:
			FixTrail(src_dir, dst_dir, filepath);
			break;
		case s2::SYM_MESH:
			FixMesh(src_dir, dst_dir, filepath);
			break;
		case s2::SYM_MASK:
			FixMask(src_dir, dst_dir, filepath);
			break;
		}
	}
}

void RectCutWithJson::RectCutImage(const std::string& src_dir, const std::string& dst_dir, 
								   const std::string& filepath, const etexpacker::ImageTrimData* trim) const
{
	std::string out_img_dir = dst_dir + "\\" + IMAGE_DIR;
	std::string out_json_dir = dst_dir + "\\" + JSON_DIR;

	auto img = ee::ImageDataMgr::Instance()->GetItem(filepath);
	if (img->GetFormat() == GPF_RGB) 
	{
		std::string filename = ee::FileHelper::GetRelativePath(src_dir, filepath);
		filename = filename.substr(0, filename.find_last_of('.'));
		ee::StringHelper::ReplaceAll(filename, "\\", "%");

		ecomplex::Symbol complex;

		std::string img_name = ee::StringHelper::Format("%s#%d#%d#%d#%d#.png", filename.c_str(), 0, 0, img->GetWidth(), img->GetHeight());
		std::string img_out_path = out_img_dir + "\\" + img_name;
		gimg_export(img_out_path.c_str(), img->GetPixelData(), img->GetWidth(), img->GetHeight(), img->GetFormat(), true);
		if (m_cfg) {
			m_cfg->AddCut(filepath, img_out_path);
		}

		std::string spr_path = std::string(out_img_dir + "\\" + img_name);
		auto spr = std::make_shared<ee::DummySprite>(std::make_shared<ee::DummySymbol>(spr_path, img->GetWidth(), img->GetHeight()));
		complex.Add(spr);

		std::string json_out_path = out_json_dir + "\\" + filename + "_complex.json";
		ecomplex::FileStorer::Store(json_out_path, complex, out_json_dir, false);

		return;
	}
	
	uint8_t* condense = NULL;
	pimg::Rect pr;
	if (img->GetFormat() == GPF_RGBA8)
	{
		pimg::Condense cd(img->GetPixelData(), img->GetWidth(), img->GetHeight());
		condense = cd.GetPixels(pr);
	}
	if (!condense) {
		pr.xmin = pr.ymin = 0;
		pr.xmax = img->GetWidth();
		pr.ymax = img->GetHeight();
	}

	std::string filename = ee::FileHelper::GetRelativePath(src_dir, filepath);
	filename = filename.substr(0, filename.find_last_of('.'));
	ee::StringHelper::ReplaceAll(filename, "\\", "%");

	ecomplex::Symbol complex;

	const uint8_t* pixel = condense ? condense : img->GetPixelData();
	eimage::RegularRectCut rect_cut(pixel, pr.Width(), pr.Height());
	rect_cut.AutoCut();

	int channels = img->GetFormat() == GPF_RGB ? 3 : 4;
	pimg::Cropping img_cut(pixel, pr.Width(), pr.Height(), channels, true);

	sm::vec2 scale(1, 1);
	sm::vec2 offset(0, 0);
	if (trim)
	{
		const etexpacker::ImageTrimData::Trim* rect = trim->Query(filepath);
		assert(rect);
		scale.x = static_cast<float>(rect->w) / img->GetWidth();
		scale.y = static_cast<float>(rect->h) / img->GetHeight();

		offset.x = rect->x + rect->w * 0.5f - rect->ori_w * 0.5f;
		offset.y = rect->ori_h * 0.5f - (rect->y + rect->h * 0.5f);
	}

	auto& rects = rect_cut.GetResult();
	for (int i = 0, n = rects.size(); i < n; ++i) 
	{
		const eimage::Rect& r = rects[i];
		const uint8_t* pixels = img_cut.Crop(r.x, r.y, r.x+r.w, r.y+r.h);

		std::string img_name = ee::StringHelper::Format("%s#%d#%d#%d#%d#.png", filename.c_str(), r.x, r.y, r.w, r.h);
		std::string img_out_path = out_img_dir + "\\" + img_name;
		gimg_export(img_out_path.c_str(), pixels, r.w, r.h, img->GetFormat(), true);
		if (m_cfg) {
			m_cfg->AddCut(filepath, img_out_path);
		}

		delete[] pixels;

		std::string spr_path = std::string(out_img_dir + "\\" + img_name);
		auto spr = std::make_shared<ee::DummySprite>(std::make_shared<ee::DummySymbol>(spr_path, r.w, r.h));
		sm::vec2 _offset;
		_offset.x = r.x + r.w * 0.5f - img->GetWidth() * 0.5f;
		_offset.y = r.y + r.h * 0.5f - img->GetHeight() * 0.5f;
		_offset *= scale;
		_offset.x += pr.xmin;
		_offset.y += pr.ymin;
		_offset += offset;
		spr->Translate(_offset);
		complex.Add(spr);
	}

	std::string json_out_path = out_json_dir + "\\" + filename + "_complex.json";
	ecomplex::FileStorer::Store(json_out_path, complex, out_json_dir, false);

	delete[] condense;
}

void RectCutWithJson::FixComplex(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	for (int i = 0, n = value["sprite"].size(); i < n; ++i) {
		FixFilepath(src_dir, dst_dir, dir, value["sprite"][i], "filepath");
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void RectCutWithJson::FixAnim(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	for (int layer_i = 0, layer_n = value["layer"].size(); layer_i < layer_n; ++layer_i) {
		Json::Value& layer_val = value["layer"][layer_i];
		for (int frame_i = 0, frame_n = layer_val["frame"].size(); frame_i < frame_n; ++frame_i) {
			Json::Value& frame_val = layer_val["frame"][frame_i];
			for (int actor_i = 0, actor_n = frame_val["actor"].size(); actor_i < actor_n; ++actor_i) {
				FixFilepath(src_dir, dst_dir, dir, frame_val["actor"][actor_i], "filepath");
			}
		}
	}	

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void RectCutWithJson::FixScale9(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	for (int i = 0, n = value["sprite"].size(); i < n; ++i) {
		FixFilepath(src_dir, dst_dir, dir, value["sprite"][i], "filepath");
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void RectCutWithJson::FixParticle3d(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	for (int i = 0, n = value["components"].size(); i < n; ++i) {
		FixFilepath(src_dir, dst_dir, dir, value["components"][i], "filepath");
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void RectCutWithJson::FixTrail(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int mode = value["mode"].asInt();
	if (mode != 0) {
		return;
	}

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	for (int i = 0, n = value["components"].size(); i < n; ++i) {
		FixFilepath(src_dir, dst_dir, dir, value["components"][i], "filepath");
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void RectCutWithJson::FixMesh(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	FixFilepath(src_dir, dst_dir, dir, value, "base_symbol");

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void RectCutWithJson::FixMask(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	FixFilepath(src_dir, dst_dir, dir, value["base"], "filepath");
	FixFilepath(src_dir, dst_dir, dir, value["mask"], "filepath");

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void RectCutWithJson::FixFilepath(const std::string& src_dir, const std::string& dst_dir,
								  const std::string& file_dir, Json::Value& val, const std::string& key) const
{
	std::string filepath = val[key].asString();
	if (filepath == ee::SYM_GROUP_TAG) {
		return FixGroup(src_dir, dst_dir, file_dir, val);
	}

	int type = ee::SymbolFile::Instance()->Type(filepath);
	if (type == s2::SYM_IMAGE) {
		FixImageFilepath(src_dir, dst_dir, file_dir, val, key);
	} else if (type == s2::SYM_MESH) {
		if (val.isMember("mesh") && val["mesh"].isMember("base_symbol")) {
			FixImageFilepath(src_dir, dst_dir, file_dir, val["mesh"], "base_symbol");
		}
		FixJsonFilepath(src_dir, dst_dir, file_dir, val, key);
	} else {
		FixJsonFilepath(src_dir, dst_dir, file_dir, val, key);
	}
}

void RectCutWithJson::FixImageFilepath(const std::string& src_dir, const std::string& dst_dir, 
									   const std::string& file_dir, Json::Value& val, const std::string& key) const
{
	std::string filepath = val[key].asString();
	filepath = ee::FileHelper::GetAbsolutePath(file_dir, filepath);
	filepath = gum::FilepathHelper::Format(filepath.c_str()).c_str();
	if (m_cfg && m_cfg->IsIgnored(filepath)) {
		return;
	}
	if (ee::FileHelper::IsFileExist(filepath)) {
		FixImageFilepathInPkg(src_dir, dst_dir, file_dir, val, key);
	} else {
		FixFilepathOutPkg(src_dir, dst_dir, file_dir, val, key);
	}
}

void RectCutWithJson::FixImageFilepathInPkg(const std::string& src_dir, const std::string& dst_dir, 
											const std::string& file_dir, Json::Value& val, const std::string& key) const
{
	std::string filepath = val[key].asString();
	filepath = ee::FileHelper::GetAbsolutePath(file_dir, filepath);
	std::string filename = ee::FileHelper::GetRelativePath(src_dir, filepath);
	filename = filename.substr(0, filename.find_last_of('.')) + "_complex.json";

	// todo
	const std::string oldVal = "\\", newVal = "%";
	for(std::string::size_type pos(0); pos != std::string::npos; pos += oldVal.length())   
	{   
		if((pos = filename.find(oldVal, pos)) != std::string::npos)
			filename.replace(pos, oldVal.length(), newVal);   
		else   
			break;   
	}  

	std::string out_json_dir = dst_dir + "\\" + JSON_DIR;
	std::string fixed_filepath = out_json_dir + "\\" + filename;

	val[key] = ee::FileHelper::GetRelativePath(file_dir, fixed_filepath);
}

void RectCutWithJson::FixFilepathOutPkg(const std::string& src_dir, const std::string& dst_dir, 
											 const std::string& file_dir, Json::Value& val, const std::string& key) const
{
	std::string filepath = val[key].asString();
	int ptr = 0;
	while (ptr + 2 < filepath.size() && filepath[ptr] == '.' && filepath[ptr + 1] == '.' && filepath[ptr + 2] == '\\') {
		ptr += 3;
	}

	std::vector<std::string> tokens;
	ee::StringHelper::Split(filepath.substr(ptr), "\\", tokens);
	if (tokens.size() < 3) {
		throw ee::Exception("Parse filepath fail: %s\n", filepath.c_str());
	}

	std::string new_path = tokens[0];
	int level = 0;
	bool find = false;
	const int MAX_LEVEL = 20;
	while (level++ < MAX_LEVEL) 
	{
		new_path = "..\\" + new_path;
		std::string dir = ee::FileHelper::GetAbsolutePath(file_dir, new_path);
		if (ee::FileHelper::IsDirExist(dir)) {
			find = true;
			break;
		}
	}

	if (!find) {
		throw ee::Exception("Can find out pkg's dir: %s\n", filepath.c_str());
	}

	//////////////////////////////////////////////////////////////////////////

	for (int i = 1, n = tokens.size(); i < n; ++i) {
		new_path += "\\" + tokens[i];		
	}

	//////////////////////////////////////////////////////////////////////////

// 	new_path += "\\_tmp_pack";
// 	std::string dir = ee::FileHelper::GetAbsolutePath(file_dir, new_path);
// 	if (!ee::FileHelper::IsDirExist(dir)) {
// 		throw ee::Exception("Out pkg should pack first, find %s fail\n", new_path.c_str());		
// 	}
// 	new_path += "\\json";
// 	dir = ee::FileHelper::GetAbsolutePath(file_dir, new_path);
// 	if (!ee::FileHelper::IsDirExist(dir)) {
// 		throw ee::Exception("Out pkg should trim images first, find %s fail\n", new_path.c_str());		
// 	}
// 	if (tokens[1] != "editor_data") {
// 		throw ee::Exception("Error filepath %s, no editor_data\n", filepath.c_str());		
// 	}
// 	new_path += "\\";
// 	for (int i = 2, n = tokens.size() - 1; i < n; ++i) {
// 		new_path += tokens[i] + "%";
// 	}
// 	new_path += tokens.back();
// 	new_path = new_path.substr(0, new_path.find_last_of('.'));
// 	new_path += "_complex.json";

	//////////////////////////////////////////////////////////////////////////

	val[key] = new_path;
}

void RectCutWithJson::FixJsonFilepath(const std::string& src_dir, const std::string& dst_dir, 
									  const std::string& file_dir, Json::Value& val, const std::string& key) const
{
	std::string filepath = val[key].asString();
	filepath = ee::FileHelper::GetAbsolutePath(file_dir, filepath);
	if (filepath.find("%") != std::string::npos) {
		return;
	}
	if (!ee::FileHelper::IsFileExist(filepath)) {
		FixFilepathOutPkg(src_dir, dst_dir, file_dir, val, key);
	}
}

void RectCutWithJson::FixGroup(const std::string& src_dir, const std::string& dst_dir, 
							   const std::string& file_dir, Json::Value& val) const
{
	for (int i = 0, n = val["group"].size(); i < n; ++i) {
		FixFilepath(src_dir, dst_dir, file_dir, val["group"][i], "filepath");
	}
}

/************************************************************************/
/* class RectCutWithJson::Config                                        */
/************************************************************************/

RectCutWithJson::Config::Config(const std::string& filepath)
	: m_filepath(filepath)
{
	Json::Value val;

	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	if (val.isNull() || val["no_cut"].isNull()) {
		return;
	}

	std::string dir = gum::FilepathHelper::Dir(filepath.c_str()).c_str();
	for (int i = 0, n = val["no_cut"].size(); i < n; ++i) 
	{
		auto path = gum::FilepathHelper::Absolute(dir.c_str(), val["no_cut"][i].asString().c_str());
		path = gum::FilepathHelper::Format(path);
		m_no_cut.insert(path.c_str());
	}

	for (int i = 0, n = val["no_compress"].size(); i < n; ++i) 
	{
		auto path = gum::FilepathHelper::Absolute(dir.c_str(), val["no_compress"][i].asString().c_str());
		path = gum::FilepathHelper::Format(path);
		m_no_compress.insert(path.c_str());
	}

	std::set<std::string>::iterator itr = m_no_cut.begin();
	for ( ; itr != m_no_cut.end(); ++itr)
	{
		std::set<std::string>::iterator itr1 = m_no_compress.find(*itr);
		if (itr1 != m_no_compress.end()) {
			m_out_no_compress.push_back(*itr);
		} else {
			m_out_compress.push_back(*itr);
		}
	}
}

bool RectCutWithJson::Config::IsIgnored(const std::string& filepath) const
{
	return m_no_cut.find(filepath) != m_no_cut.end();
}

void RectCutWithJson::Config::AddCut(const std::string& ori, const std::string& cut)
{
	std::string path = gum::FilepathHelper::Format(cut.c_str()).c_str();
	if (m_no_compress.find(ori) != m_no_compress.end()) {
		m_out_no_compress.push_back(path);
	} else {
		m_out_compress.push_back(path);
	}
}

void RectCutWithJson::Config::Ouput()
{
	std::string dir = gum::FilepathHelper::Dir(m_filepath.c_str()).c_str();
	{
		std::string filepath = dir + "\\compress.tmp";
		std::ofstream fout(filepath.c_str());
		for (int i = 0, n = m_out_compress.size(); i < n; ++i) {
			fout << m_out_compress[i] << "\n";
		}
		fout.close();
	}
	{
		std::string filepath = dir + "\\no_compress.tmp";
		std::ofstream fout(filepath.c_str());
		for (int i = 0, n = m_out_no_compress.size(); i < n; ++i) {
			fout << m_out_no_compress[i] << "\n";
		}
		fout.close();
	}
}

}