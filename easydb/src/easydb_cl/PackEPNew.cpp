#include "PackEPNew.h"
#include "check_params.h"
#include "utility.h"

#include <ee/Config.h>
#include <ee/ImageData.h>
#include <ee/FileHelper.h>

#include <easysprpacker.h>

#include <gum/FilepathHelper.h>
#include <sprite2/SprTimer.h>

namespace edb
{

std::string PackEPNew::Command() const
{
	return "pack-ep-new";
}

std::string PackEPNew::Description() const
{
	return "pack ep file";
}

std::string PackEPNew::Usage() const
{
	return Command() + " [json dir] [tp json] [tp dir] [output file] [output type] [LOD] [SCALE] [DEFAULT SYMBOL] [PLATFORM] [PKG_ID_FILE] [CURR PKG]";
}

int PackEPNew::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 8)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_folder(argv[4])) return -1;

	int LOD = 0;
	if (argc >= 8) {
		LOD = atoi(argv[7]);
	}

	float scale = 1;
	if (argc >= 9) {
		scale = atof(argv[8]);
	}

	int ret = init_gl();
	if (ret < 0) {
		return ret;
	}

	ee::Config::Instance()->EnableRender(true);

// 	Trigger(argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], LOD, scale);

	if (argc >= 10) {
		std::string default_sym = argv[9];
		if (ee::FileHelper::IsFileExist(default_sym)) {
			ee::ImageDataMgr::Instance()->SetDefaultSym(ee::FileHelper::FormatFilepathAbsolute(default_sym));
		}
	}

	if (argc >= 12) {
		esprpacker::PackIDMgr* ids_mgr = esprpacker::PackIDMgr::Instance();
		ids_mgr->Init(argv[11], argv[10]);
		ids_mgr->AddCurrPath(argv[12]);
	}

	s2::SprTimer::Instance()->Init();

	Trigger(argv[2], argv[3], argv[4], argv[5], argv[6], LOD, scale);

	return 0;
}

void PackEPNew::Trigger(const std::string& json_dir, const std::string& tp_json,
					 const std::string& tp_dir, const std::string& out_file,
					 const std::string& type, int LOD, float scale)
{
	std::string pkg_name = ee::FileHelper::GetFilename(out_file);
	std::string res_dir = gum::FilepathHelper::Format(json_dir);

	esprpacker::Packer packer(json_dir, tp_json, tp_dir);
	if (type == "lua") {
		packer.OutputLua(out_file + ".lua", scale);
	} else if (type == "ep") {
		packer.OutputEpe(out_file, true, scale);
		packer.OutputEpt(out_file, LOD, scale);
 		packer.OutputUIExtra(out_file);
 		packer.OutputSprID(pkg_name, res_dir);
	} else if (type == "epe") {
		packer.OutputEpe(out_file, true, scale);
 		packer.OutputUIExtra(out_file);
 		packer.OutputSprID(pkg_name, res_dir);
	} else if (type == "ept") {
		packer.OutputEpt(out_file, LOD, scale);
	} else if (type == "debug") {
		packer.OutputLua(out_file + ".lua", scale);
		packer.OutputEpe(out_file, true, scale);
 		packer.OutputUIExtra(out_file);
 		packer.OutputSprID(pkg_name, res_dir);
	} else if (type == "all") {
		packer.OutputLua(out_file + ".lua", scale);
		packer.OutputEpe(out_file, true, scale);
		packer.OutputEpt(out_file, LOD, scale);
 		packer.OutputUIExtra(out_file);
 		packer.OutputSprID(pkg_name, res_dir);
		packer.OutputTagKeyVal(out_file);
	}
}

}