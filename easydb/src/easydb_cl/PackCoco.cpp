#include "PackCoco.h"
#include "check_params.h"

#include <drag2d.h>
#include <easypacker.h>
#include <epbin.h>
#include <easycoco.h>

namespace edb
{

std::string PackCoco::Command() const
{
	return "pack-coco";
}

std::string PackCoco::Description() const
{
	return "pack editor's output to ep";
}

std::string PackCoco::Usage() const
{
	return Command() + " [config file path]";
}

void PackCoco::Run(int argc, char *argv[])
{
	// pack-coco E:\debug\rpack\config.json

	if (!check_number(this, argc, 3)) return;
	if (!check_file(argv[2])) return;

	Trigger(argv[2]);
}

void PackCoco::Trigger(const std::string& config_path)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(config_path.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	wxString config_dir = d2d::FilenameTools::getFileDir(config_path);

	wxString cocpackage_path = value["cocpackage.exe"].asString();

	int i = 0;
	Json::Value pkg_val = value["packages"][i++];
	while (!pkg_val.isNull()) {
//		PackTexture(pkg_val, config_dir);
//		PackLuaFile(pkg_val, config_dir);
		PackEp(pkg_val, config_dir);

		pkg_val = value["packages"][i++];
	}
}

void PackCoco::PackTexture(const Json::Value& pkg_val, const wxString& config_dir) const
{
	std::string name = pkg_val["name"].asString();
	std::string src_folder = pkg_val["src folder"].asString();
	std::string dst_folder = pkg_val["dst folder"].asString();
	std::string dst_name = config_dir + "\\" + dst_folder + "\\" + name;

	std::vector<wxString> images;
	GetAllImageFiles(pkg_val, config_dir, src_folder, images);

	libpacker::NormalPack tex_packer(images);
	tex_packer.Pack();
	std::string json_path = dst_name + "1.json";
	std::string src_folder_path = config_dir + "\\" + src_folder;
	tex_packer.OutputInfo(src_folder_path, json_path);
	std::string img_path = dst_name + "1.png";
	tex_packer.OutputImage(img_path);
}

void PackCoco::GetAllImageFiles(const Json::Value& pkg_val, const wxString& config_dir,
								const wxString& src_folder, std::vector<wxString>& images) const
{
	int i = 0;
	Json::Value src_val = pkg_val["src image list"][i++];
	while (!src_val.isNull()) {
		std::string path = src_val.asString();
		std::string path_full = config_dir + "\\" + src_folder + "\\" + path;
		if (wxFileName::DirExists(path_full)) {
			wxArrayString files;
			d2d::FilenameTools::fetchAllFiles(path_full, files);
			for (int i = 0, n = files.size(); i < n; ++i) {
				if (d2d::FileNameParser::isType(files[i], d2d::FileNameParser::e_image)) {
					images.push_back(files[i]);
				}
			}
		} else if (wxFileName::FileExists(path_full)) {
			if (d2d::FileNameParser::isType(path_full, d2d::FileNameParser::e_image)) {
				images.push_back(path_full);
			}
		}
		src_val = pkg_val["src image list"][i++];
	}
}

void PackCoco::PackLuaFile(const Json::Value& pkg_val, const wxString& config_dir) const
{
	std::string name = pkg_val["name"].asString();
	std::string src_folder = pkg_val["src folder"].asString();
	std::string dst_folder = pkg_val["dst folder"].asString();
	std::string dst_name = config_dir + "\\" + dst_folder + "\\" + name;
	std::string json_path = dst_name + "1.json";
	std::string src_folder_path = config_dir + "\\" + src_folder;
	std::string data_filter = pkg_val["src data filter"].asString();

	std::vector<wxString> files;
	GetAllDataFiles(src_folder_path, data_filter, files);
	std::vector<const d2d::ISymbol*> symbols;
 	for (int i = 0, n = files.size(); i < n; ++i) {
 		symbols.push_back(d2d::SymbolMgr::Instance()->fetchSymbol(files[i]));
 	}

	libcoco::TextureMgr tex_mgr;
	tex_mgr.Add(json_path, 0);

	libcoco::CocoPacker data_packer(symbols, tex_mgr);
	data_packer.Parser();

	std::string lua_file = dst_name + ".lua";
	data_packer.Output(lua_file.c_str());
}

void PackCoco::GetAllDataFiles(const wxString& src_folder, const wxString& filter, 
							   std::vector<wxString>& files) const
{
	std::set<wxString> unique_files;

	wxArrayString all_files;
	d2d::FilenameTools::fetchAllFiles(src_folder.ToStdString(), all_files);
	for (int i = 0, n = all_files.size(); i < n; ++i) 
	{
		wxFileName filename(all_files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (!filepath.Contains(filter)) {
			continue;
		}

		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)
			|| d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim)) {
			unique_files.insert(filepath);
		}
	}

	std::copy(unique_files.begin(), unique_files.end(), back_inserter(files));
}

void PackCoco::PackEp(const Json::Value& pkg_val, const wxString& config_dir) const
{
	std::string name = pkg_val["name"].asString();
	std::string src_folder = pkg_val["src folder"].asString();
	std::string dst_folder = pkg_val["dst folder"].asString();
	std::string dst_name = config_dir + "\\" + dst_folder + "\\" + name;
	std::string lua_file = dst_name + ".lua";
	std::string dst_folder_path = config_dir + "\\" + dst_folder;

	std::string epd_path = dst_name + ".epd";
	epbin::BinaryEPD epd(lua_file, epd_path);
	epd.Pack(true);
	std::string epp_path = dst_name + ".epp";
	epbin::BinaryEPP epp(dst_folder_path, name, epbin::TT_PNG8, epp_path);
	epp.Pack();
}

}