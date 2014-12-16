#include "NumberImages.h"
#include "check_params.h"

#include <fstream>
#include <wx/arrstr.h>
#include <drag2d.h>

namespace edb
{

std::string NumberImages::Command() const
{
	return "num-imgs";
}

std::string NumberImages::Description() const
{
	return "number image files";
}

std::string NumberImages::Usage() const
{
	return Command() + " [dir src] [dst file]";
}

void NumberImages::Run(int argc, char *argv[])
{
	// num-imgs E:\debug\character E:\debug\character\num_image.txt

	if (!check_number(this, argc, 4)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2], argv[3]);
}

void NumberImages::Trigger(const std::string& src_dir, const std::string& dst_file)
{
	std::ofstream fout(dst_file.c_str());

	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(src_dir, files);
	for (int i = 0, n = files.size(); i < n; ++i) {
		wxString filepath = files[i];
		if (!d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image)) {
			continue;
		}
		filepath = d2d::FilenameTools::getRelativePath(src_dir, filepath);
		fout << filepath.Lower() << "\n";
	}

	fout.close();
}

}