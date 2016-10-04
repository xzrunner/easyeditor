#include "UniqueImage.h"
#include "check_params.h"
#include "filetools.h"

#include <ee/FileHelper.h>
#include <ee/StringHelper.h>
#include <ee/Exception.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

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

int UniqueImage::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_folder(argv[3])) return -1;

	Trigger(argv[2], argv[3]);

	return 0;
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
	ee::FileHelper::FetchAllFiles(imgdir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::SymbolFile::Instance()->Type(filepath) == s2::SYM_IMAGE)
		{
			std::string imgpath(filepath.c_str());
			ee::StringHelper::ToLower(imgpath);

			char sig[32];
			md5_file(imgpath.c_str(), sig);
			std::string md5(reinterpret_cast<char*>(sig));
			if (md5.empty()) {
				throw ee::Exception("ProcessImageFiles md5 empty");
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
	ee::FileHelper::FetchAllFiles(jsondir, files);

	for (size_t i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::SymbolFile::Instance()->Type(filepath) == s2::SYM_ANIMATION) {
			std::string filename = filepath;
			FixImagePath(filename);
		}
	}
}

void UniqueImage::RemoveImages()
{
	for (int i = 0, n = m_to_remove.size(); i < n; ++i) {
		ee::FileHelper::RmFile(m_to_remove[i]);
	}
}

void UniqueImage::FixImagePath(const std::string& animpath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(animpath.c_str());
	std::locale::global(std::locale("C"));	
	if (fin.fail()) {
		throw ee::Exception("FixImagePath fin.fail");
	}
	reader.parse(fin, value);
	fin.close();

	Json::Value outValue = value;
	bool dirty = false;

	std::string dir = ee::FileHelper::GetFileDir(animpath);

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
				if (ee::SymbolFile::Instance()->Type(filepath) == s2::SYM_IMAGE) 
				{
					filepath = ee::FileHelper::GetAbsolutePath(dir, filepath);
					ee::StringHelper::ToLower(filepath);

					std::map<std::string, std::string>::iterator itr_img
						= m_map_image_2_md5.find(filepath);
					if (itr_img == m_map_image_2_md5.end()) {
						std::string str = "FixImagePath \""+filepath+"\""+" not exist!";
						throw ee::Exception(str.c_str());
					}

					std::map<std::string, std::string>::iterator itr_md5
						= m_map_md5_2_image.find(itr_img->second);
					if (filepath != itr_md5->second)
					{
						dirty = true;

  						const std::string& absolute = itr_md5->second;
  						std::string relative = ee::FileHelper::GetRelativePath(dir, absolute);
  						outValue["layer"][i-1]["frame"][j-1]["actor"][k-1]["filepath"] = relative;
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