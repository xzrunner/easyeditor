#include "UniqueImage.h"
#include "check_params.h"

#include <wx/wx.h>
#include <drag2d.h>

#include "filetools.h"

namespace edb
{

std::string UniqueImage::Command() const
{
	return "unique-image";
}

std::string UniqueImage::Description() const
{
	return "remove repeated images and fix links";
}

std::string UniqueImage::Usage() const
{
	return Command() + " [img path] [json path]";
}

void UniqueImage::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return;
	if (!check_folder(argv[2])) return;
	if (!check_folder(argv[3])) return;

	Trigger(argv[2], argv[3]);
}

void UniqueImage::Trigger(const std::string& imgdir, const std::string& jsondir)
{
	ProcessImageFiles(imgdir);
	ProcessJsonFiles(jsondir);
	RemoveImages();	
}

void UniqueImage::ProcessImageFiles(const std::string& imgdir)
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
			std::string imgpath(filepath.c_str());
			d2d::StringTools::ToLower(imgpath);

			char sig[32];
			md5_file(imgpath.c_str(), sig);
			std::string md5(reinterpret_cast<char*>(sig));
			if (md5.empty()) {
				throw d2d::Exception("ProcessImageFiles md5 empty");
			}

			std::map<std::string, std::string>::iterator itr_md5 
				= m_map_md5_2_image.find(md5);
			if (itr_md5 == m_map_md5_2_image.end()) {
				m_map_md5_2_image.insert(std::make_pair(md5, imgpath));
			} else {
				m_to_remove.push_back(imgpath);
				std::cout << "same img: " << itr_md5->second << " -- " << imgpath << std::endl;
			}

			std::map<std::string, std::string>::iterator itr_img
				= m_map_image_2_md5.find(imgpath);
			if (itr_img == m_map_image_2_md5.end()) {
				m_map_image_2_md5.insert(std::make_pair(imgpath, md5));
			}
		}
	}
}

void UniqueImage::ProcessJsonFiles(const std::string& jsondir)
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
			FixImagePath(filename);
		}
	}
}

void UniqueImage::RemoveImages()
{
	for (int i = 0, n = m_to_remove.size(); i < n; ++i)
	{
		wxRemoveFile(m_to_remove[i]);
	}
}

void UniqueImage::FixImagePath(const std::string& animpath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(animpath.c_str());
	if (fin.fail()) {
		throw d2d::Exception("FixImagePath fin.fail");
	}
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	Json::Value outValue = value;
	bool dirty = false;

	wxString dir = d2d::FilenameTools::getFileDir(animpath);

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
					filepath = d2d::FilenameTools::getAbsolutePath(dir, filepath);
					d2d::StringTools::ToLower(filepath);

					std::map<std::string, std::string>::iterator itr_img
						= m_map_image_2_md5.find(filepath);
					if (itr_img == m_map_image_2_md5.end()) {
						std::string str = "FixImagePath \""+filepath+"\""+" not exist!";
						throw d2d::Exception(str.c_str());
					}

					std::map<std::string, std::string>::iterator itr_md5
						= m_map_md5_2_image.find(itr_img->second);
					if (filepath != itr_md5->second)
					{
						dirty = true;

  						const wxString& absolute = itr_md5->second;
  						wxString relative = d2d::FilenameTools::getRelativePath(dir, absolute);
  						outValue["layer"][i-1]["frame"][j-1]["actor"][k-1]["filepath"] = relative.ToStdString();
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

		std::locale::global(std::locale(""));
		std::ofstream fout(animpath.c_str());
		std::locale::global(std::locale("C"));

		writer.write(fout, outValue);
		writer.write(fout, Json::Value());
		fout.close();
	}
}

}