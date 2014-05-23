#include "TrimImage.h"
#include "check_params.h"

#include <drag2d.h>
#include <easyimage.h>

namespace edb
{

std::string TrimImage::Command() const
{
	return "trim-image";
}

std::string TrimImage::Description() const
{
	return "trim image, clip blank part";
}

std::string TrimImage::Usage() const
{
	return Command() + " [dir path]";
}

void TrimImage::Run(int argc, char *argv[])
{
	// trim-image e:/test2/1001

	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);
}

void TrimImage::Trigger(const std::string& dir)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image))
		{
			d2d::Image* img = d2d::ImageMgr::Instance()->getItem(filepath);		
			eimage::ImageProcessor proc(img);
			d2d::Rect r = proc.trim();
			const unsigned char* pixels = proc.clip(r.xMin, r.xMax, r.yMin, r.yMax);
			d2d::ImageSaver::storeToFile(pixels, r.xLength(), r.yLength(), 
				filepath.ToStdString(), d2d::ImageSaver::e_png);
		}
	}
}

}