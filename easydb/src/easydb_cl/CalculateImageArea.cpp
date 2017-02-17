#include "CalculateImageArea.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/Image.h>
#include <ee/SymbolFile.h>

#include <easyimage.h>

#include <sprite2/SymType.h>

#include <wx/arrstr.h>

namespace edb
{

std::string CalImageArea::Command() const
{
	return "image-area";
}

std::string CalImageArea::Description() const
{
	return "calculate image area";
}

std::string CalImageArea::Usage() const
{
	return Command() + " [dir path]";
}

int CalImageArea::Run(int argc, char *argv[])
{
	// image-area e:/test2/1001

	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2]);

	return 0;
}

void CalImageArea::Trigger(const std::string& dir)
{
	int area = 0;

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::SymbolFile::Instance()->Type(filepath) == s2::SYM_IMAGE)
		{
			ee::Image* img = ee::ImageMgr::Instance()->GetItem(filepath);
			area += img->GetClippedRegion().Width() * img->GetClippedRegion().Height();
			img->RemoveReference();
		}
	}

	std::cout << "area: " << area << std::endl;
}

}