#include "RectCutWithJson.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/ImageData.h>
#include <ee/ImageTrim.h>
#include <ee/ImageClip.h>
#include <ee/StringHelper.h>
#include <ee/DummySprite.h>
#include <ee/ImageSaver.h>
#include <ee/DummySymbol.h>
#include <ee/DummySprite.h>

//#include <glfw.h>
#include <easyimage.h>
#include <easycomplex.h>

namespace edb
{

static const char* IMAGE_DIR = "image";
static const char* JSON_DIR = "json";

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

	Trigger(argv[2], argv[3]);

	return 0;
}

void RectCutWithJson::Trigger(const std::string& src_dir, const std::string& dst_dir)
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

		std::cout << i << " / " << n << " : " << filepath << "\n";
		if (ee::FileType::IsType(filepath, ee::FileType::e_image)) {
			RectCutImage(src_dir, dst_dir, filepath);
		} else if (ee::FileType::IsType(filepath, ee::FileType::e_complex)) {
			FixComplex(src_dir, dst_dir, filepath);
		} else if (ee::FileType::IsType(filepath, ee::FileType::e_anim)) {
			FixAnim(src_dir, dst_dir, filepath);
		} else if (ee::FileType::IsType(filepath, ee::FileType::e_scale9)) {
			FixScale9(src_dir, dst_dir, filepath);
		} else if (ee::FileType::IsType(filepath, ee::FileType::e_particle3d)) {
			FixParticle3d(src_dir, dst_dir, filepath);			
		}
	}
}

void RectCutWithJson::RectCutImage(const std::string& src_dir, const std::string& dst_dir, 
								   const std::string& filepath) const
{
	std::string out_img_dir = dst_dir + "\\" + IMAGE_DIR;
	std::string out_json_dir = dst_dir + "\\" + JSON_DIR;

	ee::ImageData* img = ee::ImageDataMgr::Instance()->GetItem(filepath);		
	
	ee::ImageTrim trim(*img);
	sm::rect img_r = trim.Trim();
	if (!img_r.IsValid()) {
		img_r.xmin = img_r.ymin = 0;
		img_r.xmax = img->GetWidth();
		img_r.ymax = img->GetHeight();
	}

	ee::ImageClip clip(*img);
	const uint8_t* pixels = clip.Clip(img_r.xmin, img_r.xmax, img_r.ymin, img_r.ymax);
	const sm::vec2& sz = img_r.Size();
	ee::ImageData* img_trimed = new ee::ImageData(pixels, sz.x, sz.y, 4);

	std::string filename = ee::FileHelper::GetRelativePath(src_dir, filepath);
	filename = filename.substr(0, filename.find_last_of('.'));
	ee::StringHelper::ReplaceAll(filename, "\\", "%");

	ecomplex::Symbol complex;

	eimage::RegularRectCut rect_cut(pixels, sz.x, sz.y);
	rect_cut.AutoCut();
	const std::vector<eimage::Rect>& rects = rect_cut.GetResult();
	ee::ImageClip img_cut(*img_trimed, true);
	for (int i = 0, n = rects.size(); i < n; ++i) 
	{
		const eimage::Rect& r = rects[i];
		const uint8_t* pixels = img_cut.Clip(r.x, r.x+r.w, r.y, r.y+r.h);

		std::string img_name = ee::StringHelper::Format("%s#%d#%d#%d#%d#.png", filename.c_str(), r.x, r.y, r.w, r.h);
		std::string img_out_path = out_img_dir + "\\" + img_name;
		ee::ImageSaver::StoreToFile(pixels, r.w, r.h, 4, img_out_path, ee::ImageSaver::e_png);
		delete[] pixels;

		std::string spr_path = std::string(out_img_dir + "\\" + img_name);
		ee::Sprite* spr = new ee::DummySprite(new ee::DummySymbol(spr_path, r.w, r.h));
		sm::vec2 offset;
		offset.x = img_r.xmin + r.x + r.w * 0.5f - img->GetWidth() * 0.5f;
		offset.y = img_r.ymin + r.y + r.h * 0.5f - img->GetHeight() * 0.5f;
		spr->Translate(offset);
		complex.Add(spr);
	}

	std::string json_out_path = out_json_dir + "\\" + filename + "_complex.json";
	ecomplex::FileStorer::Store(json_out_path.c_str(), &complex);

	img->Release();
	img_trimed->Release();
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

	int i = 0;
	Json::Value spr_val = value["sprite"][i++];
	while (!spr_val.isNull()) {
		Json::Value& val = value["sprite"][i-1];
		FixSpriteValue(src_dir, dst_dir, dir, val);
		spr_val = value["sprite"][i++];
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

	int i = 0;
	Json::Value layerVal = value["layer"][i++];
	while (!layerVal.isNull()) {
		int j = 0;
		Json::Value frameVal = layerVal["frame"][j++];
		while (!frameVal.isNull()) {
			int k = 0;
			Json::Value entryVal = frameVal["actor"][k++];
			while (!entryVal.isNull()) {
				Json::Value& val = value["layer"][i-1]["frame"][j-1]["actor"][k-1];
				FixSpriteValue(src_dir, dst_dir, dir, val);
				entryVal = frameVal["actor"][k++];
			}

			frameVal = layerVal["frame"][j++];
		}

		layerVal = value["layer"][i++];
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

	int i = 0;
	Json::Value spr_val = value["sprite"][i++];
	while (!spr_val.isNull()) {
		Json::Value& val = value["sprite"][i-1];
		FixSpriteValue(src_dir, dst_dir, dir, val);
		spr_val = value["sprite"][i++];
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

	int i = 0;
	Json::Value comp_val = value["components"][i++];
	while (!comp_val.isNull()) {
		Json::Value& val = value["components"][i-1];
		FixSpriteValue(src_dir, dst_dir, dir, val);
		comp_val = value["components"][i++];
	}		

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void RectCutWithJson::FixSpriteValue(const std::string& src_dir, const std::string& dst_dir,
									 const std::string& file_dir, Json::Value& sprite_val) const
{
	std::string filepath = sprite_val["filepath"].asString();
	if (!ee::FileType::IsType(filepath, ee::FileType::e_image)) {
		return;
	}

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
	sprite_val["filepath"] = ee::FileHelper::GetRelativePath(file_dir, fixed_filepath);
}

}