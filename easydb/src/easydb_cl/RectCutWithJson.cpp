#include "RectCutWithJson.h"
#include "check_params.h"

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

void RectCutWithJson::Run(int argc, char *argv[])
{
	// rect-cut-with-json e:/test2/1001 e:/test2/1002

	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2], argv[3]);
}

void RectCutWithJson::Trigger(const std::string& src_dir, const std::string& dst_dir)
{
	std::string out_img_dir = dst_dir + "\\" + IMAGE_DIR;
	d2d::mk_dir(out_img_dir);
	std::string out_json_dir = dst_dir + "\\" + JSON_DIR;
	d2d::mk_dir(out_json_dir);

	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(src_dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();

		std::cout << i << " / " << n << " : " << filepath << "\n";
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image)) {
			RectCutImage(src_dir, dst_dir, filepath);
		} else if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)) {
			FixComplex(src_dir, dst_dir, filepath);
		} else if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim)) {
			FixAnim(src_dir, dst_dir, filepath);
		}
	}
}

void RectCutWithJson::RectCutImage(const std::string& src_dir, const std::string& dst_dir, 
								   const std::string& filepath) const
{
	std::string out_img_dir = dst_dir + "\\" + IMAGE_DIR;
	std::string out_json_dir = dst_dir + "\\" + JSON_DIR;

	d2d::ImageData* img = d2d::ImageDataMgr::Instance()->GetItem(filepath);		
	
	eimage::ImageTrim trim(*img);
	d2d::Rect img_r = trim.Trim();
	if (!img_r.isValid()) {
		img_r.xMin = img_r.yMin = 0;
		img_r.xMax = img->GetWidth();
		img_r.yMax = img->GetHeight();
	}

	eimage::ImageClip clip(*img);
	const uint8_t* pixels = clip.Clip(img_r.xMin, img_r.xMax, img_r.yMin, img_r.yMax);
	d2d::ImageData* img_trimed = new d2d::ImageData(pixels, img_r.xLength(), img_r.yLength(), 4);

	wxString filename = d2d::FilenameTools::getRelativePath(src_dir, filepath);
	filename = filename.substr(0, filename.find_last_of('.'));
	filename.Replace("\\", "%");

	ecomplex::Symbol complex;

	eimage::RegularRectCut rect_cut(pixels, img_r.xLength(), img_r.yLength());
	rect_cut.AutoCut();
	const std::vector<eimage::Rect>& rects = rect_cut.GetResult();
	eimage::ImageClip img_cut(*img_trimed, true);
	for (int i = 0, n = rects.size(); i < n; ++i) 
	{
		const eimage::Rect& r = rects[i];
		const uint8_t* pixels = img_cut.Clip(r.x, r.x+r.w, r.y, r.y+r.h);

		wxString img_name;
		img_name.Printf("%s#%d#%d#%d#%d#.png", filename, r.x, r.y, r.w, r.h);
		std::string img_out_path = out_img_dir + "\\" + img_name;
		d2d::ImageSaver::storeToFile(pixels, r.w, r.h, 4, img_out_path, d2d::ImageSaver::e_png);
		delete[] pixels;

		std::string spr_path = std::string(out_img_dir + "\\" + img_name);
		d2d::ISprite* spr = new d2d::NullSprite(new d2d::NullSymbol(spr_path, r.w, r.h));
		d2d::Vector offset;
		offset.x = img_r.xMin + r.x + r.w * 0.5f - img->GetWidth() * 0.5f;
		offset.y = img_r.yMin + r.y + r.h * 0.5f - img->GetHeight() * 0.5f;
		spr->Translate(offset);
		complex.m_sprites.push_back(spr);
	}

	std::string json_out_path = out_json_dir + "\\" + filename + "_complex.json";
	ecomplex::FileSaver::store(json_out_path.c_str(), &complex);

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

	std::string dir = d2d::FilenameTools::getFileDir(filepath);

	int i = 0;
	Json::Value spriteVal = value["sprite"][i++];
	while (!spriteVal.isNull()) {
		Json::Value& val = value["sprite"][i-1];
		FixSpriteValue(src_dir, dst_dir, dir, val);
		spriteVal = value["sprite"][i++];
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

	std::string dir = d2d::FilenameTools::getFileDir(filepath);

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

void RectCutWithJson::FixSpriteValue(const std::string& src_dir, const std::string& dst_dir,
									 const std::string& file_dir, Json::Value& sprite_val) const
{
	std::string filepath = sprite_val["filepath"].asString();
	if (!d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image)) {
		return;
	}

	filepath = d2d::FilenameTools::getAbsolutePath(file_dir, filepath);
	
	std::string filename = d2d::FilenameTools::getRelativePath(src_dir, filepath);
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
	sprite_val["filepath"] = d2d::FilenameTools::getRelativePath(file_dir, fixed_filepath).ToStdString();
}

}