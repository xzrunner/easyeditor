#include "BinB4R.h"
#include "check_params.h"

#include <drag2d.h>
#include <epbin.h>

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
	return Command() + " [src dir] [img id file] [dst file]";
}

void BinB4R::Run(int argc, char *argv[])
{
	// bin-b4r e:\debug\cut e:\debug\cut\imgid.txt e:\debug\pack\2013baji.b4r

	if (!check_number(this, argc, 5)) return;
	if (!check_folder(argv[2])) return;
	if (!check_file(argv[3])) return;

	Trigger(argv[2], argv[3], argv[4]);
}

void BinB4R::Trigger(const std::string& src_dir, const std::string& img_id_file, 
					 const std::string& dst_file)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(src_dir, files);

	std::vector<std::string> img_files;
	for (int i = 0, n = files.size(); i < n; ++i) {
		if (d2d::FileNameParser::isType(files[i], d2d::FileNameParser::e_image)) {
			img_files.push_back(files[i].ToStdString());
		}
	}

	epbin::BinaryB4R bin(img_files, img_id_file);
	bin.Pack(dst_file, false);
}

}