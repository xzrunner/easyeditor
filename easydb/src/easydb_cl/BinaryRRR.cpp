#include "BinaryRRR.h"
#include "check_params.h"

#include <drag2d.h>
#include <epbin.h>

namespace edb
{

std::string BinaryRRR::Command() const
{
	return "bin-rrr";
}

std::string BinaryRRR::Description() const
{
	return "binary regular rect raw";
}

std::string BinaryRRR::Usage() const
{
	return Command() + " [src dir] [img id file] [dst file]";
}

void BinaryRRR::Run(int argc, char *argv[])
{
	// bin-rrr e:\debug\cut e:\debug\cut\imgid.txt e:\debug\pack\2013baji.rrr

	if (!check_number(this, argc, 5)) return;
	if (!check_folder(argv[2])) return;
	if (!check_file(argv[3])) return;

	Trigger(argv[2], argv[3], argv[4]);
}

void BinaryRRR::Trigger(const std::string& src_dir, const std::string& img_id_file, 
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

	epbin::BinaryRRR bin(img_files, img_id_file);
	bin.Pack(dst_file, false);
}

}