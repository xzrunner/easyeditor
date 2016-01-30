#include "PackTexture.h"
#include "check_params.h"

#include <drag2d.h>
#include <easytexpacker.h>

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
	std::string cmd0 = Command() + " [cfg file]";
	std::string cmd1 = Command() + " [src dir] [dst dir] [min size] [max size] [trim file] [extrude]";
	return cmd0 + " or " + cmd1;
}

void PackTexture::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;

	if (argc == 3) {
		RunFromConfig(argv[2]);
	} else {
		libtexpacker::ImageTrimData* trim = NULL;
		if (argc > 6) {
			if (strcmp(argv[6], "null") == 0) {
				trim = NULL;
			} else {
				trim = new libtexpacker::ImageTrimData(argv[6]);
			}
		}
		int extrude = 1;
		if (argc > 7) {
			extrude = atoi(argv[7]);
		}
		std::string ignore;
		if (argc > 8) {
			ignore = argv[8];
		}
		int start_id = 1;
		if (argc > 9) {
			start_id = atoi(argv[9]);
		}
		RunFromCmd(trim, argv[2], ignore, argv[3], -1, atof(argv[5]), atof(argv[4]), extrude, extrude, start_id);
	}
}

void PackTexture::RunFromConfig(const std::string& cfg_file)
{	
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(cfg_file.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = d2d::FileHelper::GetFileDir(cfg_file);

	std::string src_dir = d2d::FileHelper::GetAbsolutePath(dir, value["src"].asString());
	std::string dst_file = d2d::FileHelper::GetAbsolutePath(dir, value["dst"].asString());

 	libtexpacker::ImageTrimData* trim = NULL;
 	if (!value["trim file"].isNull()) {
 		std::string trim_file = d2d::FileHelper::GetAbsolutePath(dir, value["trim file"].asString());
 		trim = new libtexpacker::ImageTrimData(trim_file);
 	}

 	int static_size = value["static size"].asInt(),
 		max_size = value["max size"].asInt(),
 		min_size = value["min size"].asInt();
 	int extrude_min = value["extrude min"].asInt(),
 		extrude_max = value["extrude max"].asInt();

	RunFromCmd(trim, src_dir, dst_file, "", static_size, max_size, min_size, extrude_min, extrude_max, 1);

	delete trim;
}

void PackTexture::RunFromCmd(libtexpacker::ImageTrimData* trim, const std::string& src_dir, const std::string& src_ignore,
							 const std::string& dst_file, int static_size, int max_size, int min_size, int extrude_min, int extrude_max,
							 int start_id) 
{
	std::vector<std::string> images;

	wxArrayString files;
	d2d::FileHelper::FetchAllFiles(src_dir, src_ignore, files);
	for (int i = 0, n = files.size(); i < n; ++i) {
		if (d2d::FileType::IsType(files[i], d2d::FileType::e_image)) {
			std::string filepath = d2d::FileHelper::FormatFilepathAbsolute(files[i].ToStdString());
			images.push_back(filepath);
		}
	}

	std::string dst_dir = d2d::FileHelper::GetFileDir(dst_file);
	ee::FileHelper::MkDir(dst_dir);

	d2d::SettingData& sd = d2d::Config::Instance()->GetSettings();
	bool ori_cfg = sd.open_image_edge_clip;
	sd.open_image_edge_clip = false;

	libtexpacker::NormalPack tex_packer(images, trim, extrude_min, extrude_max, start_id);
	tex_packer.Pack(static_size, max_size, min_size);
	tex_packer.OutputInfo(src_dir, dst_file + ".json");
	tex_packer.OutputImage(dst_file + ".png");

	sd.open_image_edge_clip = ori_cfg;
}

}