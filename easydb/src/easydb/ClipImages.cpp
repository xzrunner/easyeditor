#include "ClipImages.h"

#include <drag2d.h>
#include <easyimage.h>

namespace edb
{

ClipImages::ClipImages(const std::string& dir)
{
	Clip(dir);
}

void ClipImages::Clip(const std::string& dir)
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