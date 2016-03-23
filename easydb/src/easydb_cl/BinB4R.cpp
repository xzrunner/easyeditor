#include "BinB4R.h"
#include "check_params.h"

#include <ee/FileHelper.h>

#include <epbin.h>

#include <wx/arrstr.h>

namespace edb
{

std::string BinB4R::Command() const
{
	return "bin-b4r";
}

std::string BinB4R::Description() const
{
	return "binary block 4 rect";
}

std::string BinB4R::Usage() const
{
	return Command() + " [src dir] [img id file] [dst file] [tex format]";
}

int BinB4R::Run(int argc, char *argv[])
{
	// bin-b4r e:\debug\cut e:\debug\cut\imgid.txt e:\debug\pack\2013baji.b4r pvr

	if (!check_number(this, argc, 6)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_file(argv[3])) return -1;

	Trigger(argv[2], argv[3], argv[4], argv[5]);

	return 0;
}

void BinB4R::Trigger(const std::string& src_dir, const std::string& img_id_file, 
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
	epbin::BinaryB4R bin(img_files, img_id_file, is_pvr);
	bin.Pack(dst_file, true);
}

}