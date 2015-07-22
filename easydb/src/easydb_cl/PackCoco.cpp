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

	std::string config_dir = d2d::FilenameTools::getFileDir(config_path);

 	std::string trim_file = value["trim file"].asString();
	if (config_dir != ".") {
		trim_file = config_dir + "\\" + trim_file;
	}
 	libpacker::ImageTrimData trim(trim_file);

	int i = 0;
	Json::Value pkg_val = value["packages"][i++];
	while (!pkg_val.isNull()) {
//		Prepare(pkg_val, config_dir);
		PackTexture(pkg_val, config_dir, trim);
//  		PackLuaFile(pkg_val, config_dir);
// 		PackBinFiles(pkg_val, config_dir);

		pkg_val = value["packages"][i++];
	}
}

void PackCoco::Prepare(const Json::Value& pkg_val, const std::string& config_dir) 
{
	std::string dst_folder = config_dir + "\\" + pkg_val["dst folder"].asString();
 	d2d::mk_dir(dst_folder);
}

void PackCoco::PackTexture(const Json::Value& pkg_val, const std::string& config_dir,
						   const libpacker::ImageTrimData& trim) const
{
	std::string name = pkg_val["name"].asString();
	std::string src_folder = pkg_val["src folder"].asString();
	std::string dst_folder = pkg_val["dst folder"].asString();
	std::string dst_name = config_dir + "\\" + dst_folder + "\\" + name;
	std::string trim_path = config_dir + "\\" + pkg_val["trim file"].asString();
	std::string tex_type = pkg_val["texture type"].asString();

	std::vector<std::string> images;
	GetAllImages(pkg_val, config_dir, src_folder, images);

	libpacker::NormalPack tex_packer(images);
	tex_packer.Pack();
	std::string json_path = dst_name + ".json";
	tex_packer.OutputInfo(src_folder, trim, json_path);

	if (pkg_val["rrp"].isNull()) {
		std::string img_path = dst_name + ".png";
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

void PackCoco::GetAllImages(const Json::Value& pkg_val, const std::string& config_dir, const std::string& src_folder, 
							std::vector<std::string>& images) const
{
	if (pkg_val["src image list"].isNull()) {
		GetImagesFromJson(config_dir, src_folder, images);
	} else {
		GetImagesFromCfg(pkg_val, config_dir, src_folder, images);
	}
}

static void _handle_sprite(const std::string& src_dir, const std::string& spr_dir, 
						   const Json::Value& spr_val, std::set<std::string>& images)
{
	std::string filepath = d2d::SymbolSearcher::GetSymbolPath(spr_dir, spr_val);
	filepath = d2d::FilenameTools::FormatFilepathAbsolute(filepath);
	if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image)) {
		filepath = d2d::FilenameTools::getRelativePath(".", filepath);
		images.insert(filepath);
	} else {
		if (filepath.find(src_dir) == std::string::npos) {
			throw d2d::Exception("_handle_sprite: file %s not in %s", filepath.c_str(), src_dir.c_str());
		}
	}
}

void PackCoco::GetImagesFromJson(const std::string& config_dir, const std::string& src_folder, 
								 std::vector<std::string>& images) const
{
	std::set<std::string> img_set;

	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(src_folder, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		std::string filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)) 
		{
			Json::Value value;
			Json::Reader reader;
			std::locale::global(std::locale(""));
			std::ifstream fin(filepath.c_str());
			std::locale::global(std::locale("C"));
			reader.parse(fin, value);
			fin.close();

			int j = 0;
			Json::Value spr_val = value["sprite"][j++];
			while (!spr_val.isNull()) {
				std::string spr_dir = d2d::FilenameTools::getFileDir(filepath);
				_handle_sprite(src_folder, spr_dir, spr_val, img_set);
				spr_val = value["sprite"][j++];
			}	
		} 
		else if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim)) 
		{
			Json::Value value;
			Json::Reader reader;
			std::locale::global(std::locale(""));
			std::ifstream fin(filepath.c_str());
			std::locale::global(std::locale("C"));
			reader.parse(fin, value);
			fin.close();

			int j = 0;
			Json::Value layer_val = value["layer"][j++];
			while (!layer_val.isNull()) {
				int k = 0;
				Json::Value frame_val = layer_val["frame"][k++];
				while (!frame_val.isNull()) {
					int m = 0;
					Json::Value spr_val = frame_val["actor"][m++];
					while (!spr_val.isNull()) {
						std::string spr_dir = d2d::FilenameTools::getFileDir(filepath);
						_handle_sprite(src_folder, spr_dir, spr_val, img_set);
						spr_val = frame_val["actor"][m++];
					}
					frame_val = layer_val["frame"][k++];
				}
				layer_val = value["layer"][j++];
			}	
		} 
		else 
		{
			throw d2d::Exception("unknown file type: %s", filepath.c_str());
		}
	}

	std::copy(img_set.begin(), img_set.end(), back_inserter(images));
}

void PackCoco::GetImagesFromCfg(const Json::Value& pkg_val, const std::string& config_dir,
								const std::string& src_folder, std::vector<std::string>& images) const
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
					images.push_back(d2d::FilenameTools::FormatFilepath(files[i].ToStdString()));
				}
			}
		} else if (wxFileName::FileExists(path_full)) {
			if (d2d::FileNameParser::isType(path_full, d2d::FileNameParser::e_image)) {
				images.push_back(d2d::FilenameTools::FormatFilepath(path_full));
			}
		}
		src_val = pkg_val["src image list"][i++];
	}
}

void PackCoco::PackLuaFile(const Json::Value& pkg_val, const std::string& config_dir) const
{
	std::string name = pkg_val["name"].asString();
	std::string src_folder = pkg_val["src folder"].asString();
	std::string dst_folder = pkg_val["dst folder"].asString();
	std::string dst_name = config_dir + "\\" + dst_folder + "\\" + name;
	std::string src_folder_path = config_dir + "\\" + src_folder;
	std::string data_filter = pkg_val["src data filter"].asString();

	std::vector<std::string> files;
	GetAllDataFiles(src_folder_path, data_filter, files);
	std::vector<const d2d::ISymbol*> symbols;
 	for (int i = 0, n = files.size(); i < n; ++i) {
 		symbols.push_back(d2d::SymbolMgr::Instance()->FetchSymbol(files[i]));
 	}

	libcoco::TextureMgr tex_mgr;
	tex_mgr.SetSrcDataDir(src_folder);
	int i = 1;
	while (true) {
		std::string path = dst_name + d2d::StringTools::IntToString(i) + ".json";
		if (d2d::FilenameTools::isExist(path)) {
			tex_mgr.Add(path, i - 1);
		} else {
			break;
		}
		++i;
	}

	libcoco::CocoPacker* data_packer = NULL;
	if (!pkg_val["rrp"].isNull() || !pkg_val["rrr"].isNull() || !pkg_val["b4r"].isNull()) {
		std::string id_filepath = config_dir + "\\" + pkg_val["id file"].asString();
		data_packer = new libcoco::CocoPacker(symbols, tex_mgr, id_filepath);	
	} else {
		data_packer = new libcoco::CocoPacker(symbols, tex_mgr);
	}

	data_packer->Parser();

	std::string lua_file = dst_name + ".lua";
	data_packer->Output(lua_file.c_str());

	delete data_packer;
}

void PackCoco::GetAllDataFiles(const std::string& src_folder, const std::string& filter, 
							   std::vector<std::string>& files) const
{
	std::set<std::string> unique_files;

	wxArrayString all_files;
	d2d::FilenameTools::fetchAllFiles(src_folder, all_files);
	for (int i = 0, n = all_files.size(); i < n; ++i) 
	{
		wxFileName filename(all_files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		if (!filter.empty() && filepath.find(filter) == std::string::npos) {
			continue;
		}

		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)
			|| d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim)) {
			unique_files.insert(filepath);
		}
	}

	std::copy(unique_files.begin(), unique_files.end(), back_inserter(files));
}

void PackCoco::PackBinFiles(const Json::Value& pkg_val, const std::string& config_dir) const
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
 		std::string id_filepath = config_dir + "\\" + pkg_val["id file"].asString();
 		epbin::BinaryRRP rrp(pack_filepath, id_filepath);
 		rrp.Pack(dst_name + ".rrp", true);
 	}

	Json::Value pts_val = pkg_val["pts"];
	if (!pts_val.isNull()) {
		std::vector<std::string> pts_files;
		GetAllPTSFiles(pkg_val, config_dir, src_folder, pts_files);

		std::string id_filepath = config_dir + "\\" + pkg_val["id file"].asString();
		std::string source_folder = config_dir + "\\" + src_folder;
		epbin::BinaryPTS pts(source_folder, pts_files, id_filepath);
		pts.Pack(dst_name + ".pts", true);
	}

	Json::Value rrr_val = pkg_val["rrr"];
	if (!rrr_val.isNull()) {
		std::string id_filepath = config_dir + "\\" + pkg_val["id file"].asString();
		std::vector<std::string> img_files;
		GetAllImages(pkg_val, config_dir, src_folder, img_files);
		
		epbin::BinaryRRR bin(img_files, id_filepath, tex_type == epbin::TT_PVR);
		bin.Pack(dst_name + ".rrr", true);
	}

	Json::Value b4r_val = pkg_val["b4r"];
	if (!b4r_val.isNull()) {
		std::string id_filepath = config_dir + "\\" + pkg_val["id file"].asString();
		std::vector<std::string> img_files;
		GetAllImages(pkg_val, config_dir, src_folder, img_files);

		epbin::BinaryB4R bin(img_files, id_filepath, tex_type == epbin::TT_PVR);
		bin.Pack(dst_name + ".b4r", true);
	}
}

void PackCoco::GetAllPTSFiles(const Json::Value& pkg_val, const std::string& config_dir, 
							  const std::string& src_folder, std::vector<std::string>& pts_files) const
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