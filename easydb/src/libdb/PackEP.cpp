#include "PackEP.h"
#include "check_params.h"
#include "utility.h"

#include <ee/Config.h>
#include <ee/ImageData.h>
#include <ee/FileHelper.h>

#include <easyrespacker.h>

namespace edb
{

std::string PackEP::Command() const
{
	return "pack-ep";
}

std::string PackEP::Description() const
{
	return "pack ep file";
}

std::string PackEP::Usage() const
{
	return Command() + " [json dir] [tp json] [tp dir] [output file] [output type] [LOD] [SCALE] [DEFAULT SYMBOL]";
}

int PackEP::Run(int argc, char *argv[])
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

	std::string default_sym;
	if (argc >= 10) {
		default_sym = argv[9];
	}
	Trigger(argv[2], argv[3], argv[4], argv[5], argv[6], LOD, scale, default_sym);

	return 0;
}

// void PackEP::Trigger(const std::string& pkgs_cfg_file,
// 					 const std::string& json_dir, const std::string& tp_json,
// 					 const std::string& tp_dir, const std::string& out_file,
// 					 const std::string& type, int LOD, float scale)
void PackEP::Trigger(const std::string& json_dir, const std::string& tp_json,
					 const std::string& tp_dir, const std::string& out_file,
					 const std::string& type, int LOD, float scale,
					 const std::string& default_sym)
{
	ee::ImageDataMgr::Instance()->SetDefaultSym(ee::FileHelper::FormatFilepathAbsolute(default_sym));

	erespacker::ResPacker packer(json_dir, tp_json, tp_dir);
	if (type == "ept_desc") {
		packer.OutputEptDesc(out_file);
		return;
	}

	packer.OutputEptDesc(out_file);

	if (type == "lua") {
		packer.OutputLua(out_file + ".lua", scale);
	} else if (type == "ep") {
		packer.OutputEpe(out_file, true, scale);
		packer.OutputEpt(out_file, LOD);
		packer.OutputEptDesc(out_file);
		packer.OutputUIExtra(out_file);
		packer.OutputSprID(out_file);
	} else if (type == "epe") {
		packer.OutputEpe(out_file, true, scale);
		packer.OutputUIExtra(out_file);
		packer.OutputSprID(out_file);
	} else if (type == "ept") {
		packer.OutputEpt(out_file, LOD);
		packer.OutputEptDesc(out_file);
	} else if (type == "debug") {
		packer.OutputLua(out_file + ".lua", scale);
		packer.OutputEpe(out_file, true, scale);
		packer.OutputUIExtra(out_file);
		packer.OutputSprID(out_file);
	} else if (type == "all") {
		packer.OutputLua(out_file + ".lua", scale);
		packer.OutputEpe(out_file, true, scale);
		packer.OutputEpt(out_file, LOD);
		packer.OutputEptDesc(out_file);
		packer.OutputUIExtra(out_file);
		packer.OutputSprID(out_file);
		packer.OutputTagKeyVal(out_file);
	}
}

}