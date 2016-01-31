#include "ImageVerticalFlip.h"
#include "check_params.h"

#include <glfw.h>

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

	return Command() + " [path]";
}

void ImageVerticalFlip::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
//	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);
}

void ImageVerticalFlip::Trigger(const std::string& path) const
{
	if (ee::FileHelper::IsFileExist(path)) {
		VerticalFlip(path);
	} else if (ee::FileHelper::IsDirExist(path)) {
		wxArrayString files;
		ee::FileHelper::FetchAllFiles(path, files);
		for (int i = 0, n = files.size(); i < n; ++i)
		{
			wxFileName filename(files[i]);
			filename.Normalize();
			std::string filepath = filename.GetFullPath().ToStdString();
			if (!ee::FileType::IsType(filepath, ee::FileType::e_image)) {
				continue;
			}

			VerticalFlip(filepath);
		}
	}
}

void ImageVerticalFlip::VerticalFlip(const std::string& filepath) const
{
	ee::ImageData* img = ee::ImageDataMgr::Instance()->GetItem(filepath);		

	eimage::ImageVerticalFlip revert(img->GetPixelData(), img->GetWidth(), img->GetHeight());
	uint8_t* pixels_revert = revert.Revert();		
	ee::ImageSaver::StoreToFile(pixels_revert, img->GetWidth(), img->GetHeight(), 
		img->GetChannels(), filepath, ee::ImageSaver::e_png);
	delete[] pixels_revert;

	img->Release();
}

}