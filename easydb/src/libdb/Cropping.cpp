#include "Cropping.h"
#include "check_params.h"

#include <ee/ImageData.h>

#include <gimg_typedef.h>
#include <gimg_export.h>
#include <pimg/Cropping.h>
#include <gum/StringHelper.h>

namespace edb
{

std::string Cropping::Command() const
{
	return "cropping";
}

std::string Cropping::Description() const
{
	return "cropping";
}

std::string Cropping::Usage() const
{
	return Command() + " [src path] [dst path] [xmin] [ymin] [xmax] [ymax]";
}

int Cropping::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 8)) return -1;
	if (!check_file(argv[2])) return -1;

	int xmin, ymin, xmax, ymax;
	gum::StringHelper::FromString(argv[4], xmin);
	gum::StringHelper::FromString(argv[5], ymin);
	gum::StringHelper::FromString(argv[6], xmax);
	gum::StringHelper::FromString(argv[7], ymax);
	Trigger(argv[2], argv[3], xmin, ymin, xmax, ymax);

	return 0;
}

void Cropping::Trigger(const std::string& src_path, const std::string& dst_path, 
					   int xmin, int ymin, int xmax, int ymax)
{
	auto img = ee::ImageDataMgr::Instance()->GetItem(src_path);

	float hw = img->GetWidth() * 0.5f;
	float hh = img->GetHeight() * 0.5f;
	xmin += hw;
	xmax += hw;
	ymin += hh;
	ymax += hh;

	int channels = img->GetFormat() == GPF_RGB ? 3 : 4;
	pimg::Cropping crop(img->GetPixelData(), img->GetWidth(), img->GetHeight(), channels, true);

	const uint8_t* pixels = crop.Crop(xmin, ymin, xmax, ymax);
	gimg_export(dst_path.c_str(), pixels, xmax - xmin, ymax - ymin, GPF_RGBA8, true);
	delete[] pixels;
}

}