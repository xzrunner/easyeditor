#include "DecodeEjoy2dRes.h"
#include "check_params.h"
#include "utility.h"

#include <ee/SettingData.h>
#include <ee/Config.h>

#include <easycomplex.h>
#include <libcoco/epe/ParserLuaFile.h>

#include <gum/RenderContext.h>

#include <algorithm>

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

#define STRINGIFY(A)  #A
#include "trans_old_ejoy2d_data.lua"

namespace edb
{

std::string DecodeEjoy2dRes::Command() const
{
	return "decode-ejoy2d";
}

std::string DecodeEjoy2dRes::Description() const
{
	return "change TexturePacker json files' value";
}

std::string DecodeEjoy2dRes::Usage() const
{
	// decode-ejoy2d e:/test2/1001/test.lua

	std::string usage = Command() + " [src lua path]";
	return usage;
}

int DecodeEjoy2dRes::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 5)) return -1;
	if (!check_folder(argv[4])) return -1;

	int ret = init_gl();
	if (ret < 0) {
		return ret;
	}
	ee::Config::Instance()->EnableRender(true);

	Run(argv[2], argv[3], argv[4]);

	return 0;
}

void DecodeEjoy2dRes::Run(const std::string& lua_path, const std::string& img_path,
						  const std::string& dst_dir)
{
	ecoco::epe::ParserLuaFile parser;
	parser.parser(lua_path);

	std::vector<std::string> img_files;
	img_files.push_back(img_path);

	ee::SettingData& setting = ee::Config::Instance()->GetSettings();
	setting.open_image_edge_clip = false;

	parser.transToEasyFiles(img_files, dst_dir);

	setting.open_image_edge_clip = true;
}

}