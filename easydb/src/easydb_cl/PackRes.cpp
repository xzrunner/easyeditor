#include "PackRes.h"
#include "check_params.h"
#include "PackTexture.h"

#include <ee/FileHelper.h>
#include <ee/Config.h>
#include <ee/SettingData.h>
#include <ee/LibpngAdapter.h>
#include <ee/StringHelper.h>
#include <ee/SymbolSearcher.h>
#include <ee/SymbolMgr.h>

#include <easytexpacker.h>
#include <easyrespacker.h>
#include <epbin.h>
#include <easycoco.h>
#include <easyimage.h>

#include <wx/filefn.h>

namespace edb
{

std::string PackRes::Command() const
{
	return "pack-coco";
}

std::string PackRes::Description() const
{
	return "pack editor's output to ep";
}

std::string PackRes::Usage() const
{
	return Command() + " [config] [dir]";
}

int PackRes::Run(int argc, char *argv[])
{
	// pack-coco data dir

	if (!check_number(this, argc, 4)) return -1;
	if (!check_file(argv[2])) return -1;
	if (!check_folder(argv[3])) return -1;

	Trigger(argv[2], argv[3]);

	return 0;
}

void PackRes::Trigger(const std::string& str_val, const std::string& dir)
{
	Json::Value value;
	Json::Reader reader;
	reader.parse(str_val, value);

	std::string config_dir = ee::FileHelper::FormatFilepathAbsolute(dir);

 	std::string trim_file = ConnectCfgDir(config_dir, value["trim file"].asString());
// 	etexpacker::ImageTrimData trim(trim_file);

	int i = 0;
	Json::Value pkg_val = value["packages"][i++];
	while (!pkg_val.isNull()) {
		Prepare(pkg_val, config_dir);
 		PackTexture(pkg_val, config_dir);

// 		// old
//  	PackLuaFile(pkg_val, config_dir);
// 		PackBinFiles(pkg_val, config_dir);

		// new 
		int LOD = pkg_val["LOD"].asInt();
		PackLuaAndBinFiles(pkg_val, config_dir, LOD);

		pkg_val = value["packages"][i++];
	}
}

void PackRes::Prepare(const Json::Value& pkg_val, const std::string& config_dir) 
{
	std::string dst_folder = ConnectCfgDir(config_dir, pkg_val["dst"].asString());
 	ee::FileHelper::MkDir(dst_folder, false);
}

void PackRes::PackTexture(const Json::Value& pkg_val, const std::string& config_dir) const
{
	ee::SettingData& sd = ee::Config::Instance()->GetSettings();
	bool ori_cfg = sd.open_image_edge_clip;
	sd.open_image_edge_clip = false;

	std::string name = pkg_val["name"].asString();
	std::string dst_folder = pkg_val["dst"].asString();
	std::string dst_name = ConnectCfgDir(config_dir, dst_folder + "\\" + name);
	std::string trim_path = ConnectCfgDir(config_dir, pkg_val["trim file"].asString());

	int extrude = 0;
	if (!pkg_val["extrude"].isNull()) {
		extrude = pkg_val["extrude"].asInt();
	}

	std::vector<std::string> images;
	GetAllImages(pkg_val, config_dir, images);

	std::string fmt = pkg_val["format"].asString();

	etexpacker::NormalPack* tex_packer = NULL;
	if (extrude != 0) {
		tex_packer = new etexpacker::NormalPack(images, NULL, extrude, extrude);
	} else {
		tex_packer = new etexpacker::NormalPack(images, NULL);
	}
	tex_packer->Pack(0);
	std::string json_path = dst_name + ".json";
	tex_packer->OutputInfo(config_dir, json_path, fmt);
	std::string img_path = dst_name + ".png";
	tex_packer->OutputImage(img_path);
	int start_id = 1;
	bool fast = pkg_val["quality"].asString() == "fast";
	PackTexture::CompressPackedTex(*tex_packer, start_id, dst_name, fmt, fast);
	delete tex_packer;

	sd.open_image_edge_clip = ori_cfg;
}

void PackRes::GetAllImages(const Json::Value& pkg_val, const std::string& config_dir, 
							std::vector<std::string>& images) const
{
	if (pkg_val["image list"].isNull()) {
		GetImagesFromJson(pkg_val, config_dir, images);
	} else {
		GetImagesFromCfg(pkg_val, config_dir, images);
	}
}

void PackRes::GetImagesFromJson(const Json::Value& pkg_val, const std::string& config_dir, 
								 std::vector<std::string>& images) const
{
	std::vector<std::string> src_dirs;
	{
		int i = 0;
		Json::Value src_val = pkg_val["src"][i++];
		while (!src_val.isNull()) {
			std::string path = ConnectCfgDir(config_dir, src_val.asString());
			if (ee::FileHelper::IsDirExist(path)) {
				ee::StringHelper::ToLower(path);
				src_dirs.push_back(path);
			}
			src_val = pkg_val["src"][i++];
		}
	}

	std::set<std::string> img_set;

	int i = 0;
	Json::Value src_val = pkg_val["src"][i++];
	while (!src_val.isNull()) {
		std::string path = ConnectCfgDir(config_dir, src_val.asString());
		if (ee::FileHelper::IsDirExist(path)) {
			wxArrayString files;
			ee::FileHelper::FetchAllFiles(path, files);
			for (int i = 0, n = files.size(); i < n; ++i) {
				GetImagesFromJson(src_dirs, files[i].ToStdString(), img_set);
			}
		} else if (ee::FileHelper::IsFileExist(path)) {
			GetImagesFromJson(src_dirs, path, img_set);
		} else {
			throw ee::Exception("PackRes::GetImagesFromJson: unknown json path %s", path.c_str());
		}
		src_val = pkg_val["src"][i++];
	}

	std::copy(img_set.begin(), img_set.end(), back_inserter(images));
}

void PackRes::GetImagesFromJson(const std::vector<std::string>& src_dirs, const std::string& filepath, 
								 std::set<std::string>& img_set) const
{
	std::string dir = ee::FileHelper::GetFileDir(filepath);

	if (ee::FileType::IsType(filepath, ee::FILE_IMAGE))
	{
		img_set.insert(filepath);
	}
	else if (ee::FileType::IsType(filepath, ee::FILE_COMPLEX)) 
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
			GetImagesFromSprite(src_dirs, dir, spr_val, img_set);
			spr_val = value["sprite"][j++];
		}	
	} 
	else if (ee::FileType::IsType(filepath, ee::FILE_ANIM)) 
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
					GetImagesFromSprite(src_dirs, dir, spr_val, img_set);
					spr_val = frame_val["actor"][m++];
				}
				frame_val = layer_val["frame"][k++];
			}
			layer_val = value["layer"][j++];
		}	
	} 
	else if (ee::FileType::IsType(filepath, ee::FILE_TEXTURE))
	{
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filepath.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		int i = 0;
		Json::Value shape_val = value["shapes"][i++];
		while (!shape_val.isNull()) {
			std::string type = shape_val["material"]["type"].asString();
			if (type == "texture") {
				std::string filepath = shape_val["material"]["texture path"].asString();
				filepath = ee::FileHelper::GetAbsolutePath(dir, filepath);
				filepath = ee::FileHelper::FormatFilepathAbsolute(filepath);
				img_set.insert(filepath);
			}
			shape_val = value["shapes"][i++];
		}
	}
	else if (ee::FileType::IsType(filepath, ee::FILE_TERRAIN2D))
	{
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filepath.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		int i = 0;
		Json::Value ocean_val = value["ocean"][i++];
		while (!ocean_val.isNull()) {
			std::string filepath = ocean_val["tex0"].asString();
			filepath = ee::FileHelper::GetAbsolutePath(dir, filepath);
			filepath = ee::FileHelper::FormatFilepathAbsolute(filepath);
			img_set.insert(filepath);
			ocean_val = value["ocean"][i++];
		}
	}
	else if (ee::FileType::IsType(filepath, ee::FILE_PARTICLE3D)) 
	{
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filepath.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		int i = 0;
		Json::Value comp_val = value["components"][i++];
		while (!comp_val.isNull()) {
			std::string filepath = comp_val["filepath"].asString();
			filepath = ee::FileHelper::GetAbsolutePath(dir, filepath);
			filepath = ee::FileHelper::FormatFilepathAbsolute(filepath);
			img_set.insert(filepath);
			comp_val = value["components"][i++];
		}
	}
	else if (ee::FileType::IsType(filepath, ee::FILE_MESH))
	{
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filepath.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		GetImagesFromJson(src_dirs, value["base_symbol"].asString(), img_set);
	}
	else if (ee::FileType::IsType(filepath, ee::FILE_MASK))
	{
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filepath.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		GetImagesFromJson(src_dirs, value["base"]["filepath"].asString(), img_set);
		GetImagesFromJson(src_dirs, value["mask"]["filepath"].asString(), img_set);
	}
	else if (filepath.find("_history.json")) 
	{
	}
	else
	{
		throw ee::Exception("PackRes::GetImagesFromJson: unknown file type: %s", filepath.c_str());
	}
}

void PackRes::GetImagesFromSprite(const std::vector<std::string>& src_dirs, const std::string& spr_dir, 
								   const Json::Value& spr_val, std::set<std::string>& images) const
{
	std::string filepath = ee::SymbolSearcher::GetSymbolPath(spr_dir, spr_val);
	filepath = ee::FileHelper::FormatFilepathAbsolute(filepath);
	if (ee::FileType::IsType(filepath, ee::FILE_IMAGE)) 
	{
//		filepath = ee::FileHelper::GetRelativePath(".", filepath);
		images.insert(filepath);
	} 
	else 
	{
		if (!src_dirs.empty())
		{
			bool find = false;
			for (int i = 0, n = src_dirs.size(); i < n; ++i) {
				if (filepath.find(src_dirs[i]) != std::string::npos) {
					find = true;
					break;
				}
			}
			if (!find) {
				std::string dirs_path;
				for (int i = 0, n = src_dirs.size(); i < n; ++i) {
					dirs_path += src_dirs[i] + ";";
				}
				throw ee::Exception("_handle_sprite: file %s not in %s", filepath.c_str(), dirs_path.c_str());
			}
		}

		GetImagesFromJson(src_dirs, filepath, images);
	}
}

void PackRes::GetImagesFromCfg(const Json::Value& pkg_val, const std::string& config_dir,
								std::vector<std::string>& images) const
{
	int i = 0;
	Json::Value src_val = pkg_val["image list"][i++];
	while (!src_val.isNull()) {
		std::string path = ConnectCfgDir(config_dir, src_val.asString());
		if (ee::FileHelper::IsDirExist(path)) {
			wxArrayString files;
			ee::FileHelper::FetchAllFiles(path, files);
			for (int i = 0, n = files.size(); i < n; ++i) {
				if (ee::FileType::IsType(files[i].ToStdString(), ee::FILE_IMAGE)) {
					images.push_back(ee::FileHelper::FormatFilepath(files[i].ToStdString()));
				}
			}
		} else if (ee::FileHelper::IsFileExist(path)) {
			if (ee::FileType::IsType(path, ee::FILE_IMAGE)) {
				images.push_back(ee::FileHelper::FormatFilepath(path));
			}
		} else {
			throw ee::Exception("PackRes::GetImagesFromCfg: unknown image path %s", path.c_str());
		}
		src_val = pkg_val["image list"][i++];
	}
}

void PackRes::PackLuaFile(const Json::Value& pkg_val, const std::string& config_dir) const
{
	std::vector<std::string> files;
	GetAllDataFiles(pkg_val, config_dir, files);
	std::vector<const ee::Symbol*> syms;
 	for (int i = 0, n = files.size(); i < n; ++i) {
 		syms.push_back(ee::SymbolMgr::Instance()->FetchSymbol(files[i]));
 	}

	std::string name = pkg_val["name"].asString();
	std::string dst_folder = pkg_val["dst"].asString();
	std::string dst_name = ConnectCfgDir(config_dir, dst_folder + "\\" + name);

	ecoco::epd::TextureMgr tex_mgr;
	tex_mgr.SetSrcDataDir(config_dir);
	int i = 1;
	while (true) {
		std::string path = dst_name + ee::StringHelper::ToString(i) + ".json";
		if (ee::FileHelper::IsFileExist(path)) {
			tex_mgr.Add(path, i - 1);
		} else {
			break;
		}
		++i;
	}

	ecoco::epd::CocoPacker* data_packer = NULL;
	if (!pkg_val["rrp"].isNull() || !pkg_val["rrr"].isNull() || !pkg_val["b4r"].isNull()) {
		std::string id_filepath = ConnectCfgDir(config_dir, pkg_val["id file"].asString());
		data_packer = new ecoco::epd::CocoPacker(syms, tex_mgr, id_filepath);	
	} else {
		data_packer = new ecoco::epd::CocoPacker(syms, tex_mgr);
	}

	data_packer->Parser();

	std::string lua_file = dst_name + ".lua";
	data_packer->Output(lua_file.c_str());

	delete data_packer;
}

void PackRes::GetAllDataFiles(const Json::Value& pkg_val, const std::string& config_dir,
							   std::vector<std::string>& files) const
{
	std::set<std::string> unique_files;

	std::string filter;
	if (!pkg_val["json filter"].isNull()) {
		filter = pkg_val["json filter"].asString();
	}

	int i = 0;
	Json::Value src_val = pkg_val["src"][i++];
	while (!src_val.isNull()) {
		std::string path = ConnectCfgDir(config_dir, src_val.asString());
		if (ee::FileHelper::IsDirExist(path)) {
			wxArrayString files;
			ee::FileHelper::FetchAllFiles(path, files);
			for (int i = 0, n = files.size(); i < n; ++i) {
				AddJsonFile(files[i].ToStdString(), filter, unique_files);
			}
		} else if (ee::FileHelper::IsFileExist(path)) {
			AddJsonFile(path, filter, unique_files);
		} else {
			throw ee::Exception("PackRes::GetAllDataFiles: unknown json path %s", path.c_str());
		}
		src_val = pkg_val["src"][i++];
	}

	std::copy(unique_files.begin(), unique_files.end(), back_inserter(files));
}

void PackRes::AddJsonFile(const std::string& filepath, const std::string& filter, 
						   std::set<std::string>& json_set) const
{
	if (!filter.empty() && filepath.find(filter) == std::string::npos) {
		return;
	}

	if (ee::FileType::IsType(filepath, ee::FILE_COMPLEX) || 
		ee::FileType::IsType(filepath, ee::FILE_ANIM)) 
	{
		json_set.insert(filepath);
	}
}

void PackRes::PackBinFiles(const Json::Value& pkg_val, const std::string& config_dir) const
{
	std::string name = pkg_val["name"].asString();
	std::string dst_folder = pkg_val["dst"].asString();
	std::string dst_name = ConnectCfgDir(config_dir, dst_folder + "\\" + name);
	std::string lua_file = dst_name + ".lua";
	std::string dst_folder_path = ConnectCfgDir(config_dir, dst_folder);

 	std::string epd_path = dst_name + ".epd";

 	epbin::BinaryEPD epd(lua_file);
 	epd.Pack(epd_path, true);

	Json::Value rrp_val = pkg_val["rrp"];
	std::string img_folder_path = dst_folder_path;
	if (!rrp_val.isNull()) {
		img_folder_path = ConnectCfgDir(config_dir, rrp_val["pack file"].asString());
	}
 	std::string epp_path = dst_name + ".epp";
	
	std::string type = pkg_val["format"].asString();
	epbin::TextureType tex_type = epbin::TT_PNG8;
	if (type == "pvr") {
		tex_type = epbin::TT_PVR;
	} else if (type == "etc1") {
		tex_type = epbin::TT_ETC1;
	}
 	epbin::BinaryEPP epp(dst_name, tex_type);
 	epp.Pack(epp_path);
 
 	if (!rrp_val.isNull()) {
 		std::string pack_filepath = ConnectCfgDir(config_dir, rrp_val["pack file"].asString());
 		std::string id_filepath = ConnectCfgDir(config_dir, pkg_val["id file"].asString());
 		epbin::BinaryRRP rrp(pack_filepath, id_filepath);
 		rrp.Pack(dst_name + ".rrp", true);
 	}

	Json::Value pts_val = pkg_val["pts"];
	if (!pts_val.isNull()) {
		std::vector<std::string> pts_files;
		GetAllPTSFiles(pkg_val, config_dir, pts_files);

		std::string id_filepath = ConnectCfgDir(config_dir, pkg_val["id file"].asString());
		epbin::BinaryPTS pts(config_dir, pts_files, id_filepath);
		pts.Pack(dst_name + ".pts", true);
	}

	Json::Value rrr_val = pkg_val["rrr"];
	if (!rrr_val.isNull()) {
		std::string id_filepath = ConnectCfgDir(config_dir, pkg_val["id file"].asString());
		std::vector<std::string> img_files;
		GetAllImages(pkg_val, config_dir, img_files);
		
		epbin::BinaryRRR bin(img_files, id_filepath, tex_type == epbin::TT_PVR);
		bin.Pack(dst_name + ".rrr", true);
	}

	Json::Value b4r_val = pkg_val["b4r"];
	if (!b4r_val.isNull()) {
		std::string id_filepath = ConnectCfgDir(config_dir, pkg_val["id file"].asString());
		std::vector<std::string> img_files;
		GetAllImages(pkg_val, config_dir, img_files);

		epbin::BinaryB4R bin(img_files, id_filepath, tex_type == epbin::TT_PVR);
		bin.Pack(dst_name + ".b4r", true);
	}
}

void PackRes::PackLuaAndBinFiles(const Json::Value& pkg_val, const std::string& config_dir, int LOD) const
{
	std::string name = pkg_val["name"].asString();
//	std::string tp_dir = ConnectCfgDir(config_dir, pkg_val["dst"].asString());
	std::string tp_dir = config_dir;
	std::string tp_name = ConnectCfgDir(config_dir, pkg_val["dst"].asString()) + "\\" + name;

	erespacker::ResPacker packer(config_dir, tp_name, tp_dir);
	packer.OutputEpe(tp_name, true);
	packer.OutputEpt(tp_name, LOD);
	packer.OutputEptDesc(tp_name);

	// debug
	packer.OutputLua(tp_name + ".lua");
}

void PackRes::GetAllPTSFiles(const Json::Value& pkg_val, const std::string& config_dir, 
							  std::vector<std::string>& pts_files) const
{
	int i = 0;
	Json::Value src_val = pkg_val["image list"][i++];
	while (!src_val.isNull()) {
		std::string path = ConnectCfgDir(config_dir, src_val.asString());
		if (ee::FileHelper::IsDirExist(path)) {
			wxArrayString files;
			ee::FileHelper::FetchAllFiles(path, files);
			for (int i = 0, n = files.size(); i < n; ++i) {
				if (ee::FileType::IsType(files[i].ToStdString(), ee::FILE_IMAGE)) {
					std::string pts_path = files[i].substr(0, files[i].find(".png")) + "_strip.json";
					if (ee::FileHelper::IsFileExist(pts_path)) {
						pts_files.push_back(pts_path);
					}
				}
			}
		} else if (ee::FileHelper::IsFileExist(path)) {
			if (ee::FileType::IsType(path, ee::FILE_IMAGE)) {
				std::string pts_path = path.substr(0, path.find(".png")) + "_strip.json";
				if (ee::FileHelper::IsFileExist(pts_path)) {
					pts_files.push_back(pts_path);
				}
			}
		}
		src_val = pkg_val["image list"][i++];
	}
}

std::string PackRes::ConnectCfgDir(const std::string& cfg_dir, const std::string& path) const
{
	if (cfg_dir == ".") {
		return path;
	} else {
		return cfg_dir + "\\" + path;
	}
}

}