#include "ImageVerticalFlip.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SymbolFile.h>
#include <ee/ImageData.h>
#include <ee/ImageVerticalFlip.h>

#include <easyanim.h>
#include <easyimage.h>

#include <gimg_typedef.h>
#include <gimg_export.h>
#include <sprite2/SymType.h>

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

int ImageVerticalFlip::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
//	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);

	return 0;
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
			std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
			if (ee::SymbolFile::Instance()->Type(filepath) != s2::SYM_IMAGE) {
				continue;
			}

			VerticalFlip(filepath);
		}
	}
}

void ImageVerticalFlip::VerticalFlip(const std::string& filepath) const
{
	ee::ImageData* img = ee::ImageDataMgr::Instance()->GetItem(filepath);		
	int format = img->GetChannels() == 3 ? GPF_RGB : GPF_RGBA;
	gimg_export(filepath.c_str(), img->GetPixelData(), img->GetWidth(), img->GetHeight(), format, false);
	img->RemoveReference();
}

}