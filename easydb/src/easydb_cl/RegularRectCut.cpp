#include "RegularRectCut.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/FileType.h>
#include <ee/SymbolMgr.h>
#include <ee/StringHelper.h>
#include <ee/ImageSymbol.h>
#include <ee/ImageClip.h>
#include <ee/Image.h>
#include <ee/ImageSaver.h>

#include <easyimage.h>

#include <glfw.h>

#include <wx/arrstr.h>

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

int RegularRectCut::Run(int argc, char *argv[])
{
	// rect-cut e:/test2/1001 e:/test2/1001

	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_folder(argv[3])) return -1;

	Trigger(argv[2], argv[3]);

	return 0;
}

void RegularRectCut::Trigger(const std::string& src_dir, const std::string& dst_dir)
{
// 	ee::ShaderMgr::Instance()->reload();

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());

		std::cout << i << " / " << n << " : " << filepath << "\n";
		if (ee::FileType::IsType(filepath, ee::FILE_IMAGE))
		{
			ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);

			ee::Image* image = static_cast<ee::ImageSymbol*>(sym)->GetImage();
			eimage::RegularRectCut cut(*image);
			cut.AutoCut();

			std::cout << ee::StringHelper::Format("File: %s, Left: %d, Used: %d", filepath.c_str(), cut.GetLeftArea(), cut.GetUseArea()) << std::endl;

			std::string filename = ee::FileHelper::GetRelativePath(src_dir, filepath);
			filename = filename.substr(0, filename.find_last_of('.'));
			ee::StringHelper::ReplaceAll(filename, "\\", "%");
			
			const std::vector<eimage::Rect>& result = cut.GetResult();
			ee::ImageClip img_cut(*image->GetImageData(), true);
			for (int i = 0, n = result.size(); i < n; ++i)
			{
				const eimage::Rect& r = result[i];
				const uint8_t* pixels = img_cut.Clip(r.x, r.x+r.w, r.y, r.y+r.h);

				std::string out_path = ee::StringHelper::Format("%s\\%s#%d#%d#%d#%d#", dst_dir.c_str(), filename.c_str(), r.x, r.y, r.w, r.h);
				ee::ImageSaver::StoreToFile(pixels, r.w, r.h, 4, out_path, ee::ImageSaver::e_png);
				delete[] pixels;
			}

			sym->RemoveReference();
		}
	}
}

}