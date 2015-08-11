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
	return "pack texture";
}

std::string PackTexture::Usage() const
{
	return Command() + " [cfg file]";
}

void PackTexture::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;

	Trigger(argv[2]);
}

void PackTexture::Trigger(const std::string& cfg_file)
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

	libpacker::ImageTrimData* trim = NULL;
	if (!value["trim file"].isNull()) {
		std::string trim_file = d2d::FilenameTools::getAbsolutePath(dir, value["trim file"].asString());
		trim = new libpacker::ImageTrimData(trim_file);
	}

	libpacker::NormalPack tex_packer(images, trim);
	tex_packer.Pack(value["static size"].asInt());
	tex_packer.OutputInfo(src_dir, dst_file + ".json");
	tex_packer.OutputImage(dst_file + ".png");

	sd.open_image_edge_clip = ori_cfg;

	delete trim;
}

}