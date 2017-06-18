#include "NumberImages.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/StringHelper.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

#include <wx/arrstr.h>

#include <fstream>

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

int NumberImages::Run(int argc, char *argv[])
{
	// num-imgs E:\debug\character E:\debug\character\num_image.txt

	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2], argv[3]);

	return 0;
}

void NumberImages::Trigger(const std::string& src_dir, const std::string& dst_file)
{
	std::locale::global(std::locale(""));	
	std::ofstream fout(dst_file.c_str());
	std::locale::global(std::locale("C"));

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, files);
	for (int i = 0, n = files.size(); i < n; ++i) {
		std::string filepath = files[i];
		if (ee::SymbolFile::Instance()->Type(filepath) != s2::SYM_IMAGE) {
			continue;
		}
		filepath = ee::FileHelper::GetRelativePath(src_dir, filepath);
		ee::StringHelper::ToLower(filepath);
		fout << filepath << "\n";
	}

	fout.close();
}

}