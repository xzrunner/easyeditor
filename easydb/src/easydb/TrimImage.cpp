#include "TrimImage.h"
#include "check_params.h"

#include <ee/SettingData.h>
#include <ee/FileHelper.h>
#include <ee/Config.h>
#include <ee/ImageData.h>
#include <ee/ImageTrim.h>
#include <ee/StringHelper.h>
#include <ee/ImageClip.h>
#include <ee/ImageSaver.h>
#include <ee/ConsoleProgressBar.h>
#include <ee/SymbolFile.h>

#include <easyimage.h>

#include <sprite2/SymType.h>

#include <wx/filename.h>

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

int TrimImage::Run(int argc, char *argv[])
{
	// trim-image e:/test2/1001 e:/test2/output

	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;

	ee::SettingData& setting = ee::Config::Instance()->GetSettings();
	bool old = setting.pre_multi_alpha;
	setting.pre_multi_alpha = false;
	Trigger(argv[2], argv[3]);
	setting.pre_multi_alpha = old;

	return 0;
}

void TrimImage::Trigger(const std::string& src_dir, const std::string& dst_dir)
{
	m_src_dir = src_dir;
	m_dst_dir = dst_dir;

	ee::FileHelper::MkDir(m_dst_dir, false);
	std::string out_json_filepath = m_dst_dir + "\\" + OUTPUT_FILE + ".json";
	if (ee::FileHelper::IsFileExist(out_json_filepath)) {
		m_json_cfg.LoadFromFile(out_json_filepath);
	}

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(m_src_dir, files);
	std::cout << "Trim image.\n";
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		ee::ConsoleProgressBar::Print(i, n);

		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();

		if (ee::SymbolFile::Instance()->Type(filepath) != s2::SYM_IMAGE) {
			continue;
		}

		int64_t img_ori_time = m_json_cfg.QueryTime(ee::FileHelper::GetRelativePath(m_src_dir, filepath)),
			img_new_time = GetFileModifyTime(filepath);

		if (img_new_time != img_ori_time) {
			Trim(filepath);
		}
	}

	m_json_cfg.OutputToFile(out_json_filepath, m_dst_dir);
}

void TrimImage::StoreBoundInfo(const ee::ImageData& img, const sm::rect& r, 
							   Json::Value& val) const
{
	// left
	{
		int tot = 0, max = 0;
		int curr = 0;
		for (int y = 0; y < img.GetHeight(); ++y) {
			if (!IsTransparent(img, r.xmin, y)) {
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
			if (!IsTransparent(img, r.xmax - 1, y)) {
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
			if (!IsTransparent(img, x, r.ymin)) {
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
			if (!IsTransparent(img, x, r.ymax - 1)) {
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

bool TrimImage::IsTransparent(const ee::ImageData& img, int x, int y) const
{
	if (img.GetChannels() != 4) {
		return false;
	} else {
		return img.GetPixelData()[(img.GetWidth() * y + x) * 4 + 3] == 0;
	}
}

void TrimImage::Trim(const std::string& filepath)
{
	ee::ImageData* img = ee::ImageDataMgr::Instance()->GetItem(filepath);

	ee::ImageTrim trim(*img);
	sm::rect r = trim.Trim();
	bool trimed = r.IsValid();
	if (!r.IsValid()) {
		r.xmin = r.ymin = 0;
		r.xmax = img->GetWidth();
		r.ymax = img->GetHeight();
	}

	// save info
	Json::Value spr_val;
	std::string relative_path = ee::FileHelper::GetRelativePath(m_src_dir, filepath);
	spr_val["filepath"] = relative_path;
	spr_val["source size"]["w"] = img->GetWidth();
	spr_val["source size"]["h"] = img->GetHeight();
	spr_val["position"]["x"] = r.xmin;
	spr_val["position"]["y"] = img->GetHeight() - r.ymax;
	const sm::vec2& sz = r.Size();
	spr_val["position"]["w"] = sz.x;
	spr_val["position"]["h"] = sz.y;
	int64_t time = GetFileModifyTime(filepath);
	spr_val["time"] = ee::StringHelper::ToString(time);
	StoreBoundInfo(*img, r, spr_val);
	m_json_cfg.Insert(relative_path, spr_val, time);

	std::string out_filepath = m_dst_dir + "\\" + relative_path,
		out_dir = ee::FileHelper::GetFileDir(out_filepath);
	ee::FileHelper::MkDir(out_dir, false);

	if (trimed) {
		ee::ImageClip clip(*img);
		const uint8_t* pixels = clip.Clip(r.xmin, r.xmax, r.ymin, r.ymax);
		ee::ImageSaver::StoreToFile(pixels, sz.x, sz.y, img->GetChannels(), 
			out_filepath, ee::ImageSaver::e_png);
		delete[] pixels;
	} else {
		ee::ImageSaver::StoreToFile(img->GetPixelData(), sz.x, sz.y, img->GetChannels(), 
			out_filepath, ee::ImageSaver::e_png);
	}

	img->RemoveReference();
}

int64_t TrimImage::GetFileModifyTime(const std::string& filepath)
{
	wxStructStat strucStat;
	wxStat(filepath, &strucStat);
	return strucStat.st_mtime;
}

//////////////////////////////////////////////////////////////////////////
// class TrimImage::JsonConfig
//////////////////////////////////////////////////////////////////////////

TrimImage::JsonConfig::
~JsonConfig()
{
	std::map<std::string, Item*>::iterator itr = m_map_items.begin();
	for ( ; itr != m_map_items.end(); ++itr) {
		delete itr->second;
	}
}

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
		Item* item = new Item;
		item->val = val;
		ee::StringHelper::FromString(val["time"].asString(), item->time);
		m_map_items.insert(std::make_pair(filepath, item));
		val = value[idx++];
	}
}

void TrimImage::JsonConfig::
Insert(const std::string& filepath, const Json::Value& val, int64_t time)
{
	std::map<std::string, Item*>::iterator itr
		= m_map_items.find(filepath);
	if (itr != m_map_items.end()) {
		assert(itr->second->time != time);
		itr->second->time = time;
		itr->second->val = val;
	} else {
		Item* item = new Item;
		item->val = val;
		item->time = time;
		item->used = true;
		m_map_items.insert(std::make_pair(filepath, item));
	}
}

void TrimImage::JsonConfig::
OutputToFile(const std::string& filepath, const std::string& dst_dir) const
{
	Json::Value value;
	std::map<std::string, Item*>::const_iterator itr 
		= m_map_items.begin();
	for (int i = 0; itr != m_map_items.end(); ++itr) {
		if (itr->second->used) {
			value[i++] = itr->second->val;
		} else {
			wxRemoveFile(dst_dir + "\\" + itr->first);
		}
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

int64_t TrimImage::JsonConfig::
QueryTime(const std::string& filepath) const
{
	std::map<std::string, Item*>::const_iterator itr
		= m_map_items.find(filepath);
	if (itr != m_map_items.end()) {
		itr->second->used = true;
		return itr->second->time;
	} else {
		return 0;
	}
}

}