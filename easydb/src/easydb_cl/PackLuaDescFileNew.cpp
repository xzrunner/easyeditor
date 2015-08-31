#include "PackLuaDescFileNew.h"
#include "check_params.h"

#include <drag2d.h>
#include <easycoco.h>

namespace edb
{

std::string PackLuaDescFileNew::Command() const
{
	return "pack-lua-desc-new";
}

std::string PackLuaDescFileNew::Description() const
{
	return "pack lua desc file";
}

std::string PackLuaDescFileNew::Usage() const
{
	return Command() + " [json dir] [tp json] [tp dir] [output file]";
}

void PackLuaDescFileNew::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 6)) return;
//	if (!check_folder(argv[2])) return;
	if (!check_folder(argv[4])) return;

	Trigger(argv[2], argv[3], argv[4], argv[5]);
}

void PackLuaDescFileNew::Trigger(const std::string& json_dir, const std::string& tp_json,
								 const std::string& tp_dir, const std::string& out_file)
{
//	libcoco::CocoPacker packer(json_dir, tp_json, tp_dir, out_file);

	libcoco::CocoLoader loader;
	loader.Load(json_dir);
}

}