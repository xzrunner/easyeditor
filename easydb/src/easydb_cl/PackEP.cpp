#include "PackEP.h"
#include "check_params.h"

#include <drag2d.h>
#include <easycoco.h>

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
	return Command() + " [json dir] [tp json] [tp dir] [output file] [output type]";
}

void PackEP::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 7)) return;
	if (!check_folder(argv[2])) return;
	if (!check_folder(argv[4])) return;

	Trigger(argv[2], argv[3], argv[4], argv[5], argv[6]);
}

void PackEP::Trigger(const std::string& json_dir, const std::string& tp_json,
					 const std::string& tp_dir, const std::string& out_file,
					 const std::string& type)
{
	libcoco::CocoPacker packer(json_dir, tp_json, tp_dir);
	if (type == "lua") {
		packer.OutputLua(out_file + ".lua");
	} else if (type == "ep") {
		packer.OutputBin(out_file, true, libcoco::TT_PNG8);
	}
}

}