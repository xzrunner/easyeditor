#include "PackEPT.h"
#include "check_params.h"

#include <ee/StringHelper.h>
#include <ee/FileHelper.h>
#include <ee/TextureFactory.h>
#include <ee/TexturePacker.h>

#include <easyrespacker.h>

namespace edb
{

std::string PackEPT::Command() const
{
	return "pack-ept";
}

std::string PackEPT::Description() const
{
	return "pack ept file";
}

std::string PackEPT::Usage() const
{
	return Command() + " [tp json] [tp dir] [output file] [output type]";
}

int PackEPT::Run(int argc, char* argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[3])) return -1;

	Trigger(argv[2], argv[3], argv[4]);

	return 0;
}

void PackEPT::Trigger(const std::string& tp_json, const std::string& tp_dir, 
					  const std::string& out_file)
{
	ee::TexturePacker tp(tp_dir);

	// load tp data
	int i = 1;
	while (true) {
		std::string tp_path = tp_json + ee::StringHelper::ToString(i) + ".json";
		if (ee::FileHelper::IsFileExist(tp_path)) {
			tp.Add(tp_path);
			ee::TextureFactory::Instance()->AddTextureFromConfig(tp_path);
		} else {
			break;
		}
		++i;
	}

	// pack
	erespacker::PackToBin::PackEPT(out_file, tp, 0);
}

}