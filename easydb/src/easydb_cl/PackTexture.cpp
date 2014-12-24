#include "PackTexture.h"
#include "check_params.h"

#include <drag2d.h>
#include <easypacker.h>

namespace edb
{

std::string PackTexture::Command() const
{
	return "pack-tex";
}

std::string PackTexture::Description() const
{
	return "texture packer";
}

std::string PackTexture::Usage() const
{
	return Command() + " [config file path]";
}

void PackTexture::Run(int argc, char *argv[])
{
	// pack-tex E:\debug\rpack\config.json

	if (!check_number(this, argc, 3)) return;
	if (!check_file(argv[2])) return;

	Trigger(argv[2]);
}

void PackTexture::Trigger(const std::string& config_path)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(config_path.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	wxString config_dir = d2d::FilenameTools::getFileDir(config_path);

	int i = 0;
	Json::Value pkg_val = value["packages"][i++];
	while (!pkg_val.isNull()) {
		std::vector<wxString> filepaths;

		std::string name = pkg_val["name"].asString();
		std::string src_folder = pkg_val["src folder"].asString();
		std::string dst_folder = pkg_val["dst folder"].asString();

		GetSrcTexFile(pkg_val, config_dir, src_folder, filepaths);

		libpacker::NormalPack packer(filepaths);
		packer.Pack();
		packer.OutputInfo(config_dir + "\\" + src_folder,
			config_dir + "\\" + dst_folder + "\\" + name + "_pack.json");
		packer.OutputImage(config_dir + "\\" + dst_folder + "\\" + name + "_pack.png");

		pkg_val = value["packages"][i++];
	}
}

void PackTexture::GetSrcTexFile(const Json::Value& pkg_val, 
								const wxString& config_dir,
								const wxString& src_folder, 
								std::vector<wxString>& filepaths) const
{
	int i = 0;
	Json::Value src_val = pkg_val["src list"][i++];
	while (!src_val.isNull()) {
		std::string path = src_val.asString();
		std::string path_all = config_dir + "\\" + src_folder + "\\" + path;
		if (wxFileName::DirExists(path_all)) {
			wxArrayString files;
			d2d::FilenameTools::fetchAllFiles(path_all, files);
			for (int i = 0, n = files.size(); i < n; ++i) {
				filepaths.push_back(files[i]);
			}
		} else if (wxFileName::FileExists(path_all)) {
			filepaths.push_back(path_all);
		}
		src_val = pkg_val["src list"][i++];
	}
}

}