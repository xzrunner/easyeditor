#include "BlockCompress.h"
#include "check_params.h"

#include <easyimage.h>

namespace edb
{

std::string BlockCompress::Command() const
{
	return "block-compress";
}

std::string BlockCompress::Description() const
{
	return "compress image by block";
}

std::string BlockCompress::Usage() const
{
	// block-compress E:\test2\image E:\test2\image\image_fixed
	return Command() + " [src dir] [dst dir]";
}

void BlockCompress::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return;
	if (!check_folder(argv[2])) return;
	if (!check_folder(argv[3])) return;

	Trigger(argv[2], argv[3]);
}


void BlockCompress::Trigger(const std::string& src_dir, const std::string& dst_dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, files);
	std::vector<std::string> filepaths;
	for (int i = 0, n = files.size(); i < n; ++i) {
		if (ee::FileType::IsType(files[i], ee::FileType::e_image)) {
			filepaths.push_back(files[i].ToStdString());
		}
	}

	eimage::BlockCompress compress(filepaths);
	compress.Compress();	
	compress.Uncompress(dst_dir);
}

}