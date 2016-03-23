#include "BinRRR.h"
#include "check_params.h"

#include <ee/FileHelper.h>

#include <epbin.h>

#include <wx/arrstr.h>

namespace edb
{

std::string BinRRR::Command() const
{
	return "bin-rrr";
}

std::string BinRRR::Description() const
{
	return "binary regular rect raw";
}

std::string BinRRR::Usage() const
{
	return Command() + " [src dir] [img id file] [dst file] [tex format]";
}

int BinRRR::Run(int argc, char *argv[])
{
	// bin-rrr e:\debug\cut e:\debug\cut\imgid.txt e:\debug\pack\2013baji.rrr pvr

	if (!check_number(this, argc, 6)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_file(argv[3])) return -1;

	Trigger(argv[2], argv[3], argv[4], argv[5]);

	return 0;
}

void BinRRR::Trigger(const std::string& src_dir, const std::string& img_id_file, 
					 const std::string& dst_file, const std::string& tex_format)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, files);
	
	std::vector<std::string> img_files;
	for (int i = 0, n = files.size(); i < n; ++i) {
		if (ee::FileType::IsType(files[i].ToStdString(), ee::FileType::e_image)) {
			img_files.push_back(files[i].ToStdString());
		}
	}

	bool is_pvr = tex_format == "pvr";
	epbin::BinaryRRR bin(img_files, img_id_file, is_pvr);
	bin.Pack(dst_file, true);
}

}