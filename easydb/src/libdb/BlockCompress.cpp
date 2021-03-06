#include "BlockCompress.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SymbolFile.h>

#include <easyimage.h>

#include <sprite2/SymType.h>

#include <wx/arrstr.h>

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

int BlockCompress::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_folder(argv[3])) return -1;

	Trigger(argv[2], argv[3]);

	return 0;
}


void BlockCompress::Trigger(const std::string& src_dir, const std::string& dst_dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, files);
	CU_VEC<std::string> filepaths;
	for (int i = 0, n = files.size(); i < n; ++i) {
		if (ee::SymbolFile::Instance()->Type(files[i].ToStdString()) == s2::SYM_IMAGE) {
			filepaths.push_back(files[i].ToStdString());
		}
	}

	eimage::BlockCompress compress(filepaths);
	compress.Compress();	
	compress.Uncompress(dst_dir);
}

}