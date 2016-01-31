#include "CalculateImageArea.h"
#include "check_params.h"

#include <glfw.h>
#include <easyimage.h>

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

void CalImageArea::Run(int argc, char *argv[])
{
	// image-area e:/test2/1001

	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);
}

void CalImageArea::Trigger(const std::string& dir)
{
	int area = 0;

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();

		if (ee::FileType::IsType(filepath, ee::FileType::e_image))
		{
			ee::Image* img = ee::ImageMgr::Instance()->GetItem(filepath);
			area += img->GetClippedWidth() * img->GetClippedHeight();
			img->Release();
		}
	}

	std::cout << "area: " << area << std::endl;
}

}