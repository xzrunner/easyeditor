#include "CalcImageArea.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/StringHelper.h>
#include <ee/SymbolFile.h>

#include <gimg_import.h>
#include <sprite2/SymType.h>

#include <wx/filename.h>

namespace edb
{

std::string CalcImageArea::Command() const
{
	return "calc-img-area";
}

std::string CalcImageArea::Description() const
{
	return "calc-img-area";
}

std::string CalcImageArea::Usage() const
{
	return Command() + " [dir]";
}

int CalcImageArea::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Run(argv[2]);

	return 0;
}

void CalcImageArea::Run(const std::string& dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);

	float area = 0;
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();

		if (ee::SymbolFile::Instance()->Type(filepath) != s2::SYM_IMAGE) {
			continue;
		}

		int w, h;
		gimg_read_header(filepath.c_str(), &w, &h);
		area += w * h;
	}

	printf("tot %f, 2048 count %f\n", area, area / 2048 / 2048);
}

}