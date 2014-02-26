#include "FixImages.h"

#include <wx/wx.h>
#include <drag2d.h>

#include "filetools.h"

namespace edb
{

FixImages::FixImages(const std::string& imgdir, const std::string& jsondir)
{
	ProcessImageFiles(imgdir);
	ProcessJsonFiles(imgdir, jsondir);
}

void FixImages::ProcessImageFiles(const std::string& imgdir)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(imgdir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image))
		{
			std::string img(filepath.c_str());
			unsigned char digest[16];
			md5_file(img.c_str(), digest);
			std::string md5(reinterpret_cast<char*>(digest));

			std::map<std::string, std::string>::iterator itr_md5 
				= _map_md5_2_image.find(md5);
			if (itr_md5 == _map_md5_2_image.end()) {
				_map_md5_2_image.insert(std::make_pair(md5, filepath));
			} else {
				wxRemoveFile(filepath);
				std::cout << "same img: " << itr_md5->second << " -- " << filepath << std::endl;
			}

			std::map<std::string, std::string>::iterator itr_img
				= _map_image_2_md5.find(img);
			if (itr_img == _map_image_2_md5.end()) {
				_map_image_2_md5.insert(std::make_pair(img, md5));
			}
		}
	}
}

void FixImages::ProcessJsonFiles(const std::string& imgdir, const std::string& jsondir)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(jsondir, files);

	for (size_t i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim)) {
			std::string filename = filepath.ToStdString();
			FixImagePath(imgdir, filename);
		}
	}
}

void FixImages::FixImagePath(const std::string& imgdir, const std::string& animpath)
{
	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(animpath.c_str());
	reader.parse(fin, value);
	fin.close();

	Json::Value outValue = value;
	bool dirty = false;

	std::string prefix = imgdir + "\\";

	int i = 0;
	Json::Value layerValue = value["layer"][i++];
	while (!layerValue.isNull()) {
		int j = 0;
		Json::Value frameValue = layerValue["frame"][j++];
		while (!frameValue.isNull()) {
			int k = 0;
			Json::Value entryValue = frameValue["actor"][k++];
			while (!entryValue.isNull()) {
				std::string filepath = entryValue["filepath"].asString();
				if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image)) 
				{
					filepath = prefix + filepath;

					std::map<std::string, std::string>::iterator itr_img
						= _map_image_2_md5.find(filepath);
					assert(itr_img != _map_image_2_md5.end());
					std::map<std::string, std::string>::iterator itr_md5
						= _map_md5_2_image.find(itr_img->second);
					if (filepath != itr_md5->second)
					{
						dirty = true;
						const std::string& full = itr_md5->second;
						std::string fixed = full.substr(full.find_first_of(prefix) + prefix.size());
						outValue["layer"][i-1]["frame"][j-1]["actor"][k-1]["filepath"] = fixed;
					}
				}
				entryValue = frameValue["actor"][k++];
			}
			frameValue = layerValue["frame"][j++];
		}
		layerValue = value["layer"][i++];
	}

	if (dirty)
	{
		Json::StyledStreamWriter writer;
		std::ofstream fout(animpath.c_str());
		writer.write(fout, outValue);
		fout.close();
	}
}

}