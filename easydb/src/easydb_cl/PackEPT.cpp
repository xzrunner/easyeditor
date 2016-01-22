#include "PackEPT.h"
#include "check_params.h"

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

void PackEPT::Run(int argc, char* argv[])
{
	if (!check_number(this, argc, 4)) return;
	if (!check_folder(argv[3])) return;

	librespacker::TextureType type = librespacker::TT_PNG8;
	char* stype = argv[5];
	if (strcmp(stype, "png") == 0) {
		type = librespacker::TT_PNG8;
	} else if (strcmp(stype, "pvr") == 0) {
		type = librespacker::TT_PVR;
	}	
	Trigger(argv[2], argv[3], argv[4], type);
}

void PackEPT::Trigger(const std::string& tp_json, const std::string& tp_dir, 
					  const std::string& out_file, librespacker::TextureType type)
{
	d2d::TexturePacker tp(tp_dir);

	// load tp data
	int i = 1;
	while (true) {
		std::string tp_path = tp_json + d2d::StringTools::ToString(i) + ".json";
		if (d2d::FilenameTools::IsFileExist(tp_path)) {
			tp.Add(tp_path);
			d2d::TextureFactory::Instance()->AddTextureFromConfig(tp_path);
		} else {
			break;
		}
		++i;
	}

	// pack
	librespacker::PackToBin::PackEPT(out_file, tp, type, 0, 1);
}

}