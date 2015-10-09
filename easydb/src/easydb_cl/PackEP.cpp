#include "PackEP.h"
#include "check_params.h"

#include <drag2d.h>
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
	return Command() + " [json dir] [tp json] [tp dir] [output file] [output type] [LOD]";
}

void PackEP::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 7)) return;
	if (!check_folder(argv[2])) return;
	if (!check_folder(argv[4])) return;

	int LOD = 0;
	if (argc == 8) {
		LOD = atoi(argv[7]);
	}
	Trigger(argv[2], argv[3], argv[4], argv[5], argv[6], LOD);
}

void PackEP::Trigger(const std::string& json_dir, const std::string& tp_json,
					 const std::string& tp_dir, const std::string& out_file,
					 const std::string& type, int LOD)
{
	librespacker::ResPacker packer(json_dir, tp_json, tp_dir);
	if (type == "lua") {
		packer.OutputLua(out_file + ".lua");
	} else if (type == "ep") {
		packer.OutputEpe(out_file, true);
		packer.OutputEpt(out_file, librespacker::TT_PNG8, LOD);
	} else if (type == "epe") {
		packer.OutputEpe(out_file, true);
	} else if (type == "ept") {
		packer.OutputEpt(out_file, librespacker::TT_PNG8, LOD);
	} else if (type == "debug") {
		packer.OutputLua(out_file + ".lua");
		packer.OutputEpe(out_file, true);
	} else if (type == "all") {
		packer.OutputLua(out_file + ".lua");
		packer.OutputEpe(out_file, true);
		packer.OutputEpt(out_file, librespacker::TT_PNG8, LOD);
	}
}

}