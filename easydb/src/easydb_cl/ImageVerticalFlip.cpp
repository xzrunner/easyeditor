#include "ImageVerticalFlip.h"
#include "check_params.h"

#include <glfw.h>
#include <drag2d.h>
#include <easyanim.h>
#include <easyimage.h>

namespace edb
{

std::string ImageVerticalFlip::Command() const
{
	return "vert-flip";
}

std::string ImageVerticalFlip::Description() const
{
	return "image vertical flip";
}

std::string ImageVerticalFlip::Usage() const
{
	// vert-flip D:\projects\ejoy\coco-tools\sg_characters_new\data\json\2003daoke\attack1\1

	return Command() + " [dir]";
}

void ImageVerticalFlip::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);
}

void ImageVerticalFlip::Trigger(const std::string& dir) const
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();
		if (!d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image)) {
			continue;
		}

		d2d::ImageData* img = d2d::ImageDataMgr::Instance()->GetItem(filepath);		

		eimage::ImageVerticalFlip revert(img->GetPixelData(), img->GetWidth(), img->GetHeight());
		uint8_t* pixels_revert = revert.Revert();		
		d2d::ImageSaver::storeToFile(pixels_revert, img->GetWidth(), img->GetHeight(), 
			img->GetChannels(), filepath, d2d::ImageSaver::e_png);
		delete[] pixels_revert;

		img->Release();
	}
}

}