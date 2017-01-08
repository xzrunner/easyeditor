#include "AverageRectCut.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/ImageData.h>
#include <ee/StringHelper.h>
#include <ee/DummySprite.h>
#include <ee/DummySymbol.h>
#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/SymbolFile.h>

#include <easycomplex.h>

#include <gimg_typedef.h>
#include <gimg_export.h>
#include <pimg/Condense.h>
#include <pimg/Cropping.h>
#include <pimg/Rect.h>
#include <sprite2/SymType.h>

namespace edb
{

static const char* IMAGE_DIR = "arc_image";
static const char* JSON_DIR = "arc_json";

std::string AverageRectCut::Command() const
{
	return "average-rect-cut";
}

std::string AverageRectCut::Description() const
{
	return "average cut image by rect";
}

std::string AverageRectCut::Usage() const
{
	return Command() + " [src dir] [dst dir]";
}

int AverageRectCut::Run(int argc, char *argv[])
{
	// average-rect-cut e:/test2/1001 e:/test2/1001 256

	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;

	ee::SettingData& setting = ee::Config::Instance()->GetSettings();
	bool old = setting.pre_multi_alpha;
	setting.pre_multi_alpha = false;
	Trigger(argv[2], argv[3], atoi(argv[4]));
	setting.pre_multi_alpha = old;

	return 0;
}

void AverageRectCut::Trigger(const std::string& src_dir, const std::string& dst_dir, int min_edge)
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
		int type = ee::SymbolFile::Instance()->Type(filepath);
		switch (type)
		{
		case s2::SYM_IMAGE:
			RectCutImage(src_dir, dst_dir, filepath, min_edge);
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
		}
	}
}

void AverageRectCut::RectCutImage(const std::string& src_dir, const std::string& dst_dir, 
								  const std::string& filepath, int min_edge) const
{
	std::string out_img_dir = dst_dir + "\\" + IMAGE_DIR;
	std::string out_json_dir = dst_dir + "\\" + JSON_DIR;

	ee::ImageData* img = ee::ImageDataMgr::Instance()->GetItem(filepath);		
	assert(img->GetFormat() == GPF_RGB || img->GetFormat() == GPF_RGBA);
	int channels = img->GetFormat() == GPF_RGB ? 3 : 4;

	uint8_t* condense = NULL;
	pimg::Rect pr;
	if (img->GetFormat() == GPF_RGBA)
	{
		pimg::Condense cd(img->GetPixelData(), img->GetWidth(), img->GetHeight());
		condense = cd.GetPixels(pr);
	}
	if (!condense) {
		pr.xmin = pr.ymin = 0;
		pr.xmax = img->GetWidth();
		pr.ymax = img->GetHeight();
	}
	const uint8_t* pixels = condense ? condense : img->GetPixelData();

	std::string filename = ee::FileHelper::GetRelativePath(src_dir, filepath);
	filename = filename.substr(0, filename.find_last_of('.'));
	ee::StringHelper::ReplaceAll(filename, "\\", "%");

	ecomplex::Symbol complex;
	pimg::Cropping img_cut(pixels, pr.Width(), pr.Height(), channels);

	int row = std::ceil((float)pr.Width() / min_edge),
		col = std::ceil((float)pr.Height() / min_edge);
	float xedge = (float)pr.Width() / col,
		  yedge = (float)pr.Height()  / row;
	for (int y = 0; y < row; ++y) {
		for (int x = 0; x < col; ++x) {
			int xmin = std::floor(xedge * x),
				ymin = std::floor(yedge * y);
			int xmax = std::floor(xedge * (x + 1)),
				ymax = std::floor(yedge * (y + 1));
			if (x == col - 1) {
				xmax = pr.Width();
			}
			if (y == row - 1) {
				ymax = pr.Height();
			}

			int w = xmax - xmin,
				h = ymax - ymin;
			const uint8_t* pixels = img_cut.Crop(xmin, ymin, xmax, ymax);

			std::string img_name = ee::StringHelper::Format("%s#%d#%d#%d#%d#.png", filename.c_str(), xmin, ymin, w, h);
			std::string img_out_path = out_img_dir + "\\" + img_name;
			gimg_export(img_out_path.c_str(), pixels, w, h, GPF_RGBA, true);
			delete[] pixels;

			std::string spr_path = std::string(out_img_dir + "\\" + img_name);
			ee::Sprite* spr = new ee::DummySprite(new ee::DummySymbol(spr_path, w, h));
			sm::vec2 offset;
			offset.x = pr.xmin + xmin + w * 0.5f - img->GetWidth() * 0.5f;
			offset.y = pr.ymin + ymin + h * 0.5f - img->GetHeight() * 0.5f;
			spr->Translate(offset);
			complex.Add(spr);
		}
	}

	delete[] condense;

	std::string json_out_path = out_json_dir + "\\" + filename + "_complex.json";
	ecomplex::FileStorer::Store(json_out_path, &complex, out_json_dir);

	img->RemoveReference();
}

void AverageRectCut::FixComplex(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const
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

void AverageRectCut::FixAnim(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const
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

void AverageRectCut::FixScale9(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const
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

void AverageRectCut::FixSpriteValue(const std::string& src_dir, const std::string& dst_dir,
									 const std::string& file_dir, Json::Value& sprite_val) const
{
	std::string filepath = sprite_val["filepath"].asString();
	if (ee::SymbolFile::Instance()->Type(filepath) != s2::SYM_IMAGE) {
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