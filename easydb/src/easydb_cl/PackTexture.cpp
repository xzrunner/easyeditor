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
	std::string cmd1 = Command() + " [src dir] [dst dir] [min size] [max size] [trim file]";
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
			trim = new libtexpacker::ImageTrimData(argv[6]);
		}
		RunFromCmd(trim, argv[2], argv[3], -1, atof(argv[5]), atof(argv[4]), 1, 1);
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

	std::string dir = d2d::FilenameTools::getFileDir(cfg_file);

	std::string src_dir = d2d::FilenameTools::getAbsolutePath(dir, value["src"].asString());
	std::string dst_file = d2d::FilenameTools::getAbsolutePath(dir, value["dst"].asString());

 	libtexpacker::ImageTrimData* trim = NULL;
 	if (!value["trim file"].isNull()) {
 		std::string trim_file = d2d::FilenameTools::getAbsolutePath(dir, value["trim file"].asString());
 		trim = new libtexpacker::ImageTrimData(trim_file);
 	}

 	int static_size = value["static size"].asInt(),
 		max_size = value["max size"].asInt(),
 		min_size = value["min size"].asInt();
 	int extrude_min = value["extrude min"].asInt(),
 		extrude_max = value["extrude max"].asInt();

	RunFromCmd(trim, src_dir, dst_file, static_size, max_size, min_size, extrude_min, extrude_max);

	delete trim;
}

void PackTexture::RunFromCmd(libtexpacker::ImageTrimData* trim, const std::string& src_dir, const std::string& dst_file, 
							 int static_size, int max_size, int min_size, int extrude_min, int extrude_max) 
{
	std::vector<std::string> images;

	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(src_dir, files);
	for (int i = 0, n = files.size(); i < n; ++i) {
		if (d2d::FileNameParser::isType(files[i], d2d::FileNameParser::e_image)) {
			std::string filepath = d2d::FilenameTools::FormatFilepathAbsolute(files[i].ToStdString());
			images.push_back(filepath);
		}
	}

	std::string dst_dir = d2d::FilenameTools::getFileDir(dst_file);
	d2d::mk_dir(dst_dir);

	d2d::SettingData& sd = d2d::Config::Instance()->GetSettings();
	bool ori_cfg = sd.open_image_edge_clip;
	sd.open_image_edge_clip = false;

	libtexpacker::NormalPack tex_packer(images, trim, extrude_min, extrude_max);
	tex_packer.Pack(static_size, max_size, min_size);
	tex_packer.OutputInfo(src_dir, dst_file + ".json");
	tex_packer.OutputImage(dst_file + ".png");

	sd.open_image_edge_clip = ori_cfg;
}

}