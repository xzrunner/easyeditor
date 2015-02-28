#include "PackCoco.h"
#include "check_params.h"

#include <drag2d.h>
#include <easypacker.h>
#include <epbin.h>
#include <easycoco.h>
#include <easyimage.h>

#include <wx/dir.h>

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

 	std::string trim_file = config_dir + "\\" + value["trim file"].asString();
 	libpacker::ImageTrimData trim(trim_file);

	int i = 0;
	Json::Value pkg_val = value["packages"][i++];
	while (!pkg_val.isNull()) {
		Prepare(pkg_val, config_dir);
 		PackTexture(pkg_val, config_dir, trim);
 		PackLuaFile(pkg_val, config_dir);
		PackBinFiles(pkg_val, config_dir);

		pkg_val = value["packages"][i++];
	}
}

void PackCoco::Prepare(const Json::Value& pkg_val, const wxString& config_dir) 
{
	std::string dst_folder = config_dir + "\\" + pkg_val["dst folder"].asString();
 	d2d::MkDirRF(dst_folder);
}

void PackCoco::PackTexture(const Json::Value& pkg_val, const wxString& config_dir,
						   const libpacker::ImageTrimData& trim) const
{
	std::string name = pkg_val["name"].asString();
	std::string src_folder = pkg_val["src folder"].asString();
	std::string dst_folder = pkg_val["dst folder"].asString();
	std::string dst_name = config_dir + "\\" + dst_folder + "\\" + name;
	std::string trim_path = config_dir + "\\" + pkg_val["trim file"].asString();
	std::string tex_type = pkg_val["texture type"].asString();

	std::vector<std::string> images;
	GetAllImageFiles(pkg_val, config_dir, src_folder, images);

	libpacker::NormalPack tex_packer(images);
	tex_packer.Pack();
	std::string json_path = dst_name + "1.json";
	std::string src_folder_path = config_dir + "\\" + src_folder;
	tex_packer.OutputInfo(src_folder_path, trim, json_path);

	if (pkg_val["rrp"].isNull()) {
		std::string img_path = dst_name + "1.png";
		tex_packer.OutputImage(img_path);
		CompressTexture(img_path, tex_type);
//		wxRemoveFile(img_path);
	}
}

void PackCoco::CompressTexture(const std::string& filepath, const std::string& type) const
{
	if (type == "png") {
		return;
	}

	int w, h, c, f;
	uint8_t* pixels = eimage::ImageIO::Read(filepath.c_str(), w, h, c, f);
	if (type == "pvr") {
		std::string out_file = filepath.substr(0, filepath.find_last_of('.')) + ".pvr";
		eimage::TransToPVR trans(pixels, w, h, c, true);
		trans.OutputFile(out_file);	
	} else if (type == "etc1") {
		std::string out_file = filepath.substr(0, filepath.find_last_of('.'));
		eimage::TransToETC1 trans(pixels, w, h, c);
		trans.OutputFile(out_file);					
	}
	delete[] pixels;
}

void PackCoco::GetAllImageFiles(const Json::Value& pkg_val, const wxString& config_dir,
								const wxString& src_folder, std::vector<std::string>& images) const
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
					images.push_back(files[i].ToStdString());
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

	libcoco::CocoPacker* data_packer = NULL;
	if (!pkg_val["rrp"].isNull()) {
		std::string id_filepath = config_dir + "\\" + pkg_val["rrp"]["id file"].asString();
		data_packer = new libcoco::CocoPacker(symbols, tex_mgr, id_filepath);
	} else if (!pkg_val["rrr"].isNull()) {
		std::string id_filepath = config_dir + "\\" + pkg_val["rrr"]["id file"].asString();
		data_packer = new libcoco::CocoPacker(symbols, tex_mgr, id_filepath);
	} else {
		data_packer = new libcoco::CocoPacker(symbols, tex_mgr);
	}

	data_packer->Parser();

	std::string lua_file = dst_name + ".lua";
	data_packer->Output(lua_file.c_str());

	delete data_packer;
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

void PackCoco::PackBinFiles(const Json::Value& pkg_val, const wxString& config_dir) const
{
	std::string name = pkg_val["name"].asString();
	std::string src_folder = pkg_val["src folder"].asString();
	std::string dst_folder = pkg_val["dst folder"].asString();
	std::string dst_name = config_dir + "\\" + dst_folder + "\\" + name;
	std::string lua_file = dst_name + ".lua";
	std::string dst_folder_path = config_dir + "\\" + dst_folder;

 	std::string epd_path = dst_name + ".epd";
 	epbin::BinaryEPD epd(lua_file);
 	epd.Pack(epd_path, true);

	Json::Value rrp_val = pkg_val["rrp"];
	std::string img_folder_path = dst_folder_path;
	if (!rrp_val.isNull()) {
		img_folder_path = config_dir + "\\" + rrp_val["pack file"].asString();
	}
 	std::string epp_path = dst_name + ".epp";
	
	std::string type = pkg_val["texture type"].asString();
	epbin::TextureType tex_type = epbin::TT_PNG8;
	if (type == "pvr") {
		tex_type = epbin::TT_PVR;
	} else if (type == "etc1") {
		tex_type = epbin::TT_PKM;
	}
 	epbin::BinaryEPP epp(dst_name, tex_type);
 	epp.Pack(epp_path);
 
 	if (!rrp_val.isNull()) {
 		std::string pack_filepath = config_dir + "\\" + rrp_val["pack file"].asString();
 		std::string id_filepath = config_dir + "\\" + rrp_val["id file"].asString();
 		epbin::BinaryRRP rrp(pack_filepath, id_filepath);
 		rrp.Pack(dst_name + ".rrp", true);
 	}

	Json::Value pts_val = pkg_val["pts"];
	if (!pts_val.isNull()) {
		std::vector<std::string> pts_files;
		GetAllPTSFiles(pkg_val, config_dir, src_folder, pts_files);

		std::string id_filepath = config_dir + "\\" + pts_val["id file"].asString();
		std::string source_folder = config_dir + "\\" + src_folder;
		epbin::BinaryPTS pts(source_folder, pts_files, id_filepath);
		pts.Pack(dst_name + ".pts", true);
	}

	Json::Value rrr_val = pkg_val["rrr"];
	if (!rrr_val.isNull()) {
		std::string id_filepath = config_dir + "\\" + rrr_val["id file"].asString();
		std::vector<std::string> img_files;
		GetAllImageFiles(pkg_val, config_dir, src_folder, img_files);
		
		epbin::BinaryRRR bin(img_files, id_filepath);
		bin.Pack(dst_name + ".rrr", true);
	}
}

void PackCoco::GetAllPTSFiles(const Json::Value& pkg_val, const wxString& config_dir, 
							  const wxString& src_folder, std::vector<std::string>& pts_files) const
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
					std::string pts_path = files[i].substr(0, files[i].find(".png")) + "_strip.json";
					if (d2d::FilenameTools::isExist(pts_path)) {
						pts_files.push_back(pts_path);
					}
				}
			}
		} else if (wxFileName::FileExists(path_full)) {
			if (d2d::FileNameParser::isType(path_full, d2d::FileNameParser::e_image)) {
				std::string pts_path = path_full.substr(0, path_full.find(".png")) + "_strip.json";
				if (d2d::FilenameTools::isExist(pts_path)) {
					pts_files.push_back(pts_path);
				}
			}
		}
		src_val = pkg_val["src image list"][i++];
	}
}

}