#include "TrimImage.h"
#include "check_params.h"

#include <easyimage.h>

namespace edb
{

static const char* OUTPUT_FILE = "trim";

std::string TrimImage::Command() const
{
	return "trim-image";
}

std::string TrimImage::Description() const
{
	return "trim image, clip blank part";
}

std::string TrimImage::Usage() const
{
	return Command() + " [src dir] [dst dir]";
}

void TrimImage::Run(int argc, char *argv[])
{
	// trim-image e:/test2/1001 e:/test2/output

	if (!check_number(this, argc, 4)) return;
	if (!check_folder(argv[2])) return;

	d2d::SettingData& setting = d2d::Config::Instance()->GetSettings();
	bool old = setting.pre_multi_alpha;
	setting.pre_multi_alpha = false;
	Trigger(argv[2], argv[3]);
	setting.pre_multi_alpha = old;
}

void TrimImage::Trigger(const std::string& src_dir, const std::string& dst_dir)
{
	JsonConfig json_cfg;

	d2d::mk_dir(dst_dir, false);
	std::string out_json_filepath = dst_dir + "\\" + OUTPUT_FILE + ".json";
	wxDateTime json_time;
	json_time.Set(0.0f);
	if (d2d::FilenameTools::IsFileExist(out_json_filepath)) {
		json_cfg.LoadFromFile(out_json_filepath);
		wxStructStat strucStat;
		wxStat(out_json_filepath, &strucStat);
		json_time = strucStat.st_mtime;
	}

	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(src_dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image))
		{
			std::cout << i << " / " << n << " : " << filepath << "\n";

			wxStructStat strucStat;
			wxStat(filepath, &strucStat);
			wxDateTime img_time = strucStat.st_mtime;
			if (img_time < json_time) {
				continue;
			}

			d2d::ImageData* img = d2d::ImageDataMgr::Instance()->GetItem(filepath);		

			eimage::ImageTrim trim(*img);
			d2d::Rect r = trim.Trim();
			if (!r.isValid()) {
				r.xMin = r.yMin = 0;
				r.xMax = img->GetWidth();
				r.yMax = img->GetHeight();
			}

			// save info
			Json::Value spr_val;
			std::string relative_path = d2d::FilenameTools::getRelativePath(src_dir, filepath);
			spr_val["filepath"] = relative_path;
			spr_val["source size"]["w"] = img->GetWidth();
			spr_val["source size"]["h"] = img->GetHeight();
			spr_val["position"]["x"] = r.xMin;
			spr_val["position"]["y"] = img->GetHeight() - r.yMax;
			spr_val["position"]["w"] = r.xLength();
			spr_val["position"]["h"] = r.yLength();
			StoreBoundInfo(*img, r, spr_val);
			json_cfg.Insert(relative_path, spr_val);

			std::string out_filepath = dst_dir + "\\" + relative_path,
				out_dir = d2d::FilenameTools::getFileDir(out_filepath);
			d2d::mk_dir(out_dir, false);

			eimage::ImageClip clip(*img);
			const uint8_t* pixels = clip.Clip(r.xMin, r.xMax, r.yMin, r.yMax);
			d2d::ImageSaver::storeToFile(pixels, r.xLength(), r.yLength(), img->GetChannels(), 
				out_filepath, d2d::ImageSaver::e_png);
			delete[] pixels;

			img->Release();
		}
	}

	json_cfg.OutputToFile(out_json_filepath);
}

void TrimImage::StoreBoundInfo(const d2d::ImageData& img, const d2d::Rect& r, 
							   Json::Value& val) const
{
	// left
	{
		int tot = 0, max = 0;
		int curr = 0;
		for (int y = 0; y < img.GetHeight(); ++y) {
			if (!IsTransparent(img, r.xMin, y)) {
				++curr;
				++tot;
				if (curr > max) {
					max = curr;
				} else {
					curr = 0;
				}
			}
		}
		val["bound"]["left"]["tot"] = tot;
		val["bound"]["left"]["max"] = max;
	}
	// right
	{
		int tot = 0, max = 0;
		int curr = 0;
		for (int y = 0; y < img.GetHeight(); ++y) {
			if (!IsTransparent(img, r.xMax - 1, y)) {
				++curr;
				++tot;
				if (curr > max) {
					max = curr;
				} else {
					curr = 0;
				}
			}
		}
		val["bound"]["right"]["tot"] = tot;
		val["bound"]["right"]["max"] = max;
	}
	// bottom
	{
		int tot = 0, max = 0;
		int curr = 0;
		for (int x = 0; x < img.GetWidth(); ++x) {
			if (!IsTransparent(img, x, r.yMin)) {
				++curr;
				++tot;
				if (curr > max) {
					max = curr;
				} else {
					curr = 0;
				}
			}
		}
		val["bound"]["bottom"]["tot"] = tot;
		val["bound"]["bottom"]["max"] = max;
	}
	// up
	{
		int tot = 0, max = 0;
		int curr = 0;
		for (int x = 0; x < img.GetWidth(); ++x) {
			if (!IsTransparent(img, x, r.yMax - 1)) {
				++curr;
				++tot;
				if (curr > max) {
					max = curr;
				} else {
					curr = 0;
				}
			}
		}
		val["bound"]["up"]["tot"] = tot;
		val["bound"]["up"]["max"] = max;
	}
}

bool TrimImage::IsTransparent(const d2d::ImageData& img, int x, int y) const
{
	if (img.GetChannels() != 4) {
		return false;
	} else {
		return img.GetPixelData()[(img.GetWidth() * y + x) * 4 + 3] == 0;
	}
}

//////////////////////////////////////////////////////////////////////////
// class TrimImage::JsonConfig
//////////////////////////////////////////////////////////////////////////

void TrimImage::JsonConfig::
LoadFromFile(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int idx = 0;
	Json::Value val = value[idx++];
	while (!val.isNull()) {
		std::string filepath = val["filepath"].asString();
		m_map.insert(std::make_pair(filepath, val));
		val = value[idx++];
	}
}

void TrimImage::JsonConfig::
Insert(const std::string& filepath, const Json::Value& val)
{
	std::map<std::string, Json::Value>::iterator itr
		= m_map.find(filepath);
	if (itr != m_map.end()) {
		m_map.erase(itr);
	}
	m_map.insert(std::make_pair(filepath, val));
}

void TrimImage::JsonConfig::
OutputToFile(const std::string& filepath) const
{
	Json::Value value;
	std::map<std::string, Json::Value>::const_iterator itr 
		= m_map.begin();
	for (int i = 0; itr != m_map.end(); ++itr, ++i) {
		value[i] = itr->second;
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

}