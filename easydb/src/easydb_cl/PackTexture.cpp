#include <gl/glew.h>

#include "PackTexture.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/ShaderMgr.h>

#include <easytexpacker.h>

#include <glfw.h>

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
	std::string cmd0 = Command() + " [cfg file]";
	std::string cmd1 = Command() + " [src dir] [dst dir] [min size] [max size] [trim file] [extrude]";
	return cmd0 + " or " + cmd1;
}

int PackTexture::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;

	glfwInit();
	if(!glfwOpenWindow(100, 100, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
	{
		glfwTerminate();
		return -2;
	}

	if (glewInit() != GLEW_OK) {
		return -2;
	}

	ee::ShaderMgr::Instance();

	if (argc == 3) {
		RunFromConfig(argv[2]);
	} else {
		etexpacker::ImageTrimData* trim = NULL;
		if (argc > 6) {
			if (strcmp(argv[6], "null") == 0) {
				trim = NULL;
			} else {
				trim = new etexpacker::ImageTrimData(argv[6]);
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

	return 0;
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

	std::string dir = ee::FileHelper::GetFileDir(cfg_file);

	std::string src_dir = ee::FileHelper::GetAbsolutePath(dir, value["src"].asString());
	std::string dst_file = ee::FileHelper::GetAbsolutePath(dir, value["dst"].asString());

 	etexpacker::ImageTrimData* trim = NULL;
 	if (!value["trim file"].isNull()) {
 		std::string trim_file = ee::FileHelper::GetAbsolutePath(dir, value["trim file"].asString());
 		trim = new etexpacker::ImageTrimData(trim_file);
 	}

 	int static_size = value["static size"].asInt(),
 		max_size = value["max size"].asInt(),
 		min_size = value["min size"].asInt();
 	int extrude_min = value["extrude min"].asInt(),
 		extrude_max = value["extrude max"].asInt();

	RunFromCmd(trim, src_dir, dst_file, "", static_size, max_size, min_size, extrude_min, extrude_max, 1);

	delete trim;
}

void PackTexture::RunFromCmd(etexpacker::ImageTrimData* trim, const std::string& src_dir, const std::string& src_ignore,
							 const std::string& dst_file, int static_size, int max_size, int min_size, int extrude_min, int extrude_max,
							 int start_id) 
{
	std::vector<std::string> images;

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, src_ignore, files);
	for (int i = 0, n = files.size(); i < n; ++i) {
		if (ee::FileType::IsType(files[i].ToStdString(), ee::FileType::e_image)) {
			std::string filepath = ee::FileHelper::FormatFilepathAbsolute(files[i].ToStdString());
			images.push_back(filepath);
		}
	}

	std::string dst_dir = ee::FileHelper::GetFileDir(dst_file);
	ee::FileHelper::MkDir(dst_dir);

	ee::SettingData& sd = ee::Config::Instance()->GetSettings();
	bool ori_cfg = sd.open_image_edge_clip;
	sd.open_image_edge_clip = false;

	etexpacker::NormalPack tex_packer(images, trim, extrude_min, extrude_max, start_id);
	tex_packer.Pack(static_size, max_size, min_size);
	tex_packer.OutputInfo(src_dir, dst_file + ".json");
	tex_packer.OutputImage(dst_file + ".png");

	sd.open_image_edge_clip = ori_cfg;
}

}