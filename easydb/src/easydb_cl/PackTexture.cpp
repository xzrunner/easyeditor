#include "PackTexture.h"
#include "check_params.h"
#include "utility.h"

#include <ee/FileHelper.h>
#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/EE_ShaderLab.h>
#include <ee/StringHelper.h>
#include <ee/LibpngAdapter.h>

#include <easytexpacker.h>
#include <easyimage.h>

#include <wx/filefn.h>

#include <fstream>

namespace edb
{

std::string PackTexture::Command() const
{
	return "pack-tex";
}

std::string PackTexture::Description() const
{
	return "pack texture";
}

std::string PackTexture::Usage() const
{
	std::string cmd0 = Command() + " [json str]";
	std::string cmd1 = Command() + " [src dir] [dst dir] [min size] [max size] [trim file] [extrude]";
	return cmd0 + " or " + cmd1;
}

int PackTexture::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;

	int ret = init_gl();
	if (ret < 0) {
		return ret;
	}

	// one param
	if (argc == 3) 
	{
		std::vector<Package> packages;
		std::string src_dir, dst_file;
		etexpacker::ImageTrimData* trim = PreparePackages(argv[2], packages, src_dir, dst_file);
		Pack(packages, src_dir, dst_file);
		delete trim;
	}
	// multi params
	else 
	{
		Package pkg;

		pkg.sources.push_back(argv[2]);
		pkg.format = "png";

		pkg.size_min = atof(argv[4]);
		pkg.size_max = atof(argv[5]);

		if (argc > 6) {
			if (strcmp(argv[6], "null") != 0) {
				pkg.trim = new etexpacker::ImageTrimData(argv[6]);
			}
		}

		int extrude = 1;
		if (argc > 7) {
			extrude = atoi(argv[7]);
		}
		pkg.extrude_min = pkg.extrude_max = extrude;

		std::vector<Package> packages;
		packages.push_back(pkg);
		std::string dst_file = argv[3];
		Pack(packages, argv[2], dst_file);

		delete pkg.trim;
	}

	return 0;
}

void PackTexture::Pack(const std::vector<Package>& packages, const std::string& src_dir, const std::string& dst_file)
{
	std::string dst_dir = ee::FileHelper::GetFileDir(dst_file);
	ee::FileHelper::MkDir(dst_dir);

	ee::SettingData& sd = ee::Config::Instance()->GetSettings();
	bool ori_cfg = sd.open_image_edge_clip;
	sd.open_image_edge_clip = false;

	int start_id = 1;
	for (int i = 0, n = packages.size(); i < n; ++i) {
		PackPackage(packages[i], src_dir, dst_file, start_id);
	}

	sd.open_image_edge_clip = ori_cfg;
}

etexpacker::ImageTrimData* PackTexture::PreparePackages(const std::string& str, std::vector<Package>& packages, 
														std::string& src_dir, std::string& dst_file)
{
	Json::Value value;
	Json::Reader reader;
	reader.parse(str, value);

	src_dir = value["src"].asString();
	dst_file = value["dst"].asString();
	etexpacker::ImageTrimData* trim = NULL;
	if (!value["trim_file"].isNull()) {
		std::string trim_file = value["trim_file"].asString();
		trim = new etexpacker::ImageTrimData(trim_file);
	}

	const Json::Value& pkgs_val = value["packages"];
	for (int i = 0, n = pkgs_val.size(); i < n; ++i)
	{
		const Json::Value& v = pkgs_val[i];
		Package pkg;
		if (v["src"].isString()) {
			pkg.sources.push_back(v["src"].asString());
		} else {
			assert(v["src"].isArray());
			for (int i = 0, n = v["src"].size(); i < n; ++i) {
				pkg.sources.push_back(v["src"][i].asString());
			}
		}
		pkg.format = v["format"].asString();
		pkg.quality = v["quality"].asString();
		pkg.size_max = v["size_max"].asInt();
		pkg.size_min = v["size_min"].asInt();
		pkg.trim = trim;
		pkg.extrude_min = v["extrude_min"].asInt();
		pkg.extrude_max = v["extrude_max"].asInt();
		packages.push_back(pkg);
	}

	for (int i = 0, n = packages.size(); i < n; ++i) {
		Package& pkg0 = packages[i];
		if (pkg0.sources.size() == 1 && pkg0.sources[0] == "others") 
		{
			for (int j = 0; j < n; ++j) 
			{
				if (j == i) continue;
				const Package& pkg1 = packages[j];
				if (pkg1.sources.size() == 1 && pkg1.sources[0] == "others")  {
					continue;
				}
				for (int k = 0, m = pkg1.sources.size(); k < m; ++k) {
					const std::string& path = pkg1.sources[k];
					if (path != "others" && path != src_dir) {
						pkg0.ignores.push_back(path);
					}
				}
			}
			pkg0.sources[0] = src_dir;
		}
	}

	return trim;
}

void PackTexture::PackPackage(const Package& pkg, const std::string& src_dir, 
							  const std::string& dst_file, int& start_id)
{
	std::vector<std::string> images;

	wxArrayString files;
	if (pkg.ignores.empty()) {
		for (int i = 0, n = pkg.sources.size(); i < n; ++i) {
			ee::FileHelper::FetchAllFiles(pkg.sources[i], files);
		}
	} else {
		for (int i = 0, n = pkg.sources.size(); i < n; ++i) {
			ee::FileHelper::FetchAllFiles(pkg.sources[i], pkg.ignores, files);
		}
	}
	for (int i = 0, n = files.size(); i < n; ++i) {
		if (ee::FileType::IsType(files[i].ToStdString(), ee::FileType::e_image)) {
			std::string filepath = ee::FileHelper::FormatFilepathAbsolute(files[i].ToStdString());
			images.push_back(filepath);
		}
	}

	etexpacker::NormalPack tex_packer(images, pkg.trim, pkg.extrude_min, pkg.extrude_max, start_id);
	tex_packer.Pack(0, pkg.size_max, pkg.size_min);
	tex_packer.OutputInfo(src_dir, dst_file + ".json", pkg.format);
	tex_packer.OutputImage(dst_file + ".png");
	int begin = start_id;
	start_id += tex_packer.DstTexCount();
	int end = start_id;
	
	if (pkg.format == "pvr") 
	{
		for (int i = begin; i < end; ++i) 
		{
			std::string src = dst_file + ee::StringHelper::ToString(i) + ".png";
			std::string dst = dst_file + ee::StringHelper::ToString(i) + ".pvr";

			int w, h, c, f;
			uint8_t* pixels = ee::LibpngAdapter::Read(src.c_str(), w, h, c, f);
			eimage::TransToPVR trans(pixels, w, h, c, false, pkg.quality == "fastest");

			trans.OutputFile(dst);
			wxRemoveFile(src);
		}
	}
}

}