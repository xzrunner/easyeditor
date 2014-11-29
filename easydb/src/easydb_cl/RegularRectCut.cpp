#include "RegularRectCut.h"
#include "check_params.h"

#include <glfw.h>
#include <easyimage.h>

namespace edb
{

std::string RegularRectCut::Command() const
{
	return "rect-cut";
}

std::string RegularRectCut::Description() const
{
	return "cut image by rect";
}

std::string RegularRectCut::Usage() const
{
	return Command() + " [dir path] [dst path]";
}

void RegularRectCut::Run(int argc, char *argv[])
{
	// rect-cut e:/test2/1001 e:/test2/1001

	if (!check_number(this, argc, 4)) return;
	if (!check_folder(argv[2])) return;
	if (!check_folder(argv[3])) return;

	Trigger(argv[2], argv[3]);
}

void RegularRectCut::Trigger(const std::string& src_dir, const std::string& dst_dir)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(src_dir, files);
	int idx = 0;
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();

		std::cout << i << " / " << n << " : " << filepath << "\n";

		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image))
		{
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);

			d2d::Image* image = static_cast<d2d::ImageSymbol*>(symbol)->getImage();
			eimage::RegularRectCut cut(*image);
			cut.AutoCut();

			const std::vector<eimage::RegularRectCut::Rect>& result = cut.GetResult();
			eimage::ImageProcessor img_cut(image);
			for (int i = 0, n = result.size(); i < n; ++i)
			{
				const eimage::RegularRectCut::Rect& r = result[i];
				const unsigned char* pixels = img_cut.clip(r.x, r.x+r.edge, r.y, r.y+r.edge);
				std::string out_path = dst_dir + "\\" + wxString::FromDouble(idx++);
				d2d::ImageSaver::storeToFile(pixels, r.edge, r.edge, out_path, d2d::ImageSaver::e_png);
				delete[] pixels;
			}

			symbol->Release();
		}
	}
}

}