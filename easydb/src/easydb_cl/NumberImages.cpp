#include "NumberImages.h"
#include "check_params.h"

#include <fstream>
#include <wx/arrstr.h>


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
	ee::FileHelper::FetchAllFiles(src_dir, files);
	for (int i = 0, n = files.size(); i < n; ++i) {
		wxString filepath = files[i];
		if (!ee::FileType::IsType(filepath, ee::FileType::e_image)) {
			continue;
		}
		filepath = ee::FileHelper::GetRelativePath(src_dir, filepath);
		fout << filepath.Lower() << "\n";
	}

	fout.close();
}

}