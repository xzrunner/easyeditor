#include "VerifyImage.h"
#include "check_params.h"

#include <wx/wx.h>
#include <drag2d.h>

namespace edb 
{

std::string VerifyImage::Command() const
{
	return "verify-image";
}

std::string VerifyImage::Description() const
{
	return "check if image file lack or surplus";
}

std::string VerifyImage::Usage() const
{
	return Command() + " [dir path] (-r)or(--remove)";
}

void VerifyImage::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;
	
	if (argc >= 4) {
		_delete_surplus = check_params(argv[3], "-r", "--remove");
	} else {
		_delete_surplus = false;
	}

	Trigger(argv[2]);
}

void VerifyImage::Trigger(const std::string& dirpath)
{
	InitFiles(dirpath);
	VerifyLack();
	VerifySurplus();
	Report();
}

void VerifyImage::InitFiles(const std::string& dirpath)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dirpath, files);

	for (size_t i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)) {
			std::string filename = filepath.ToStdString();
			d2d::StringTools::ToLower(filename);
			_complex_files.push_back(filename);
		} else if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim)) {
			std::string filename = filepath.ToStdString();
			d2d::StringTools::ToLower(filename);
			_anim_files.push_back(filename);
		} else if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image)) {
			std::string filename = filepath.ToStdString();
			d2d::StringTools::ToLower(filename);
			_map_images.insert(std::make_pair(filename, false));
		}
	}
}

void VerifyImage::VerifyLack()
{
	for (size_t i = 0, n = _complex_files.size(); i < n; ++i)
	{
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(_complex_files[i].c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		int j = 0;
		Json::Value spriteValue = value["sprite"][j++];
		while (!spriteValue.isNull()) {
			std::string base = _complex_files[i];
			std::string relative = spriteValue["filepath"].asString();
			std::string filepath = d2d::FilenameTools::getAbsolutePathFromFile(base, relative).ToStdString();
			d2d::StringTools::ToLower(filepath);
			HandleSpritePath(filepath);

			spriteValue = value["sprite"][j++];
		}		
	}
	for (size_t i = 0, n = _anim_files.size(); i < n; ++i)
	{
		const std::string& anim = _anim_files[i];

		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(anim.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		int j = 0;
		Json::Value layerValue = value["layer"][j++];
		while (!layerValue.isNull()) {
			int i = 0;
			Json::Value frameValue = layerValue["frame"][i++];
			while (!frameValue.isNull()) {
				int j = 0;
				Json::Value entryValue = frameValue["actor"][j++];
				while (!entryValue.isNull()) {
					std::string relative = entryValue["filepath"].asString();
					std::string filepath = d2d::FilenameTools::getAbsolutePathFromFile(anim, relative).ToStdString();
					d2d::StringTools::ToLower(filepath);
					HandleSpritePath(filepath);

					entryValue = frameValue["actor"][j++];
				}
				frameValue = layerValue["frame"][i++];
			}
			layerValue = value["layer"][j++];
		}
	}
}

void VerifyImage::VerifySurplus()
{
	std::map<std::string, bool>::iterator itr = _map_images.begin();
	for ( ; itr != _map_images.end(); ++itr) {
		if (!itr->second) {
			_reports.insert("Surplus Image " + itr->first);
			if (_delete_surplus) {
				wxRemoveFile(itr->first);
			}
		}
	}
}

void VerifyImage::Report() const
{
	if (_reports.empty()) {
		std::cout << "Images OK!" << std::endl;
	} else {
		std::set<std::string>::const_iterator itr = _reports.begin();
		for ( ; itr != _reports.end(); ++itr)
			std::cout << *itr << std::endl;
	}
}

void VerifyImage::HandleSpritePath(const std::string& filepath)
{
	if (!d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image))
		return;

	std::map<std::string, bool>::iterator itr = _map_images.find(filepath);
	if (itr == _map_images.end())
		_reports.insert("Lack Image " + filepath);
	else
		itr->second = true;
}

}