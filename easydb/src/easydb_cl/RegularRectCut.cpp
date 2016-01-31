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
// 	ee::ShaderMgr::Instance()->reload();

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();

		std::cout << i << " / " << n << " : " << filepath << "\n";
		if (ee::FileType::IsType(filepath, ee::FileType::e_image))
		{
			ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);

			ee::Image* image = static_cast<ee::ImageSymbol*>(symbol)->GetImage();
			eimage::RegularRectCut cut(*image);
			cut.AutoCut();

			wxString msg;
			msg.Printf("File: %s, Left: %d, Used: %d", filepath, cut.GetLeftArea(), cut.GetUseArea());
			std::cout << msg << std::endl;

			wxString filename = ee::FileHelper::GetRelativePath(src_dir, filepath);
			filename = filename.substr(0, filename.find_last_of('.'));
			filename.Replace("\\", "%");

			const std::vector<eimage::Rect>& result = cut.GetResult();
			eimage::ImageClip img_cut(*image->GetImageData(), true);
			for (int i = 0, n = result.size(); i < n; ++i)
			{
				const eimage::Rect& r = result[i];
				const uint8_t* pixels = img_cut.Clip(r.x, r.x+r.w, r.y, r.y+r.h);

				wxString out_path;
				out_path.Printf("%s\\%s#%d#%d#%d#%d#", dst_dir, filename, r.x, r.y, r.w, r.h);
				ee::ImageSaver::StoreToFile(pixels, r.w, r.h, 4, out_path.ToStdString(), ee::ImageSaver::e_png);
				delete[] pixels;
			}

			symbol->Release();
		}
	}
}

}