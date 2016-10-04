#include "TransToPVR.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/LibpngAdapter.h>
#include <ee/SymbolFile.h>

#include <easyimage.h>
//#include <dtex_pvr.h>

#include <sprite2/SymType.h>

namespace edb
{

std::string TransToPVR::Command() const
{
	return "trans2pvr";
}

std::string TransToPVR::Description() const
{
	return "trans images to pvr";
}

std::string TransToPVR::Usage() const
{
	return Command() + " [path]";
}

int TransToPVR::Run(int argc, char *argv[])
{
	// trans2pvr e:/test2/1001

	if (!check_number(this, argc, 3)) return -1;

	Trigger(argv[2]);

	return 0;
}

void TransToPVR::Trigger(const std::string& path)
{
	if (ee::FileHelper::IsDirExist(path)) 
	{
		wxArrayString files;
		ee::FileHelper::FetchAllFiles(path, files);
		for (int i = 0, n = files.size(); i < n; ++i)
		{
			std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
			if (ee::SymbolFile::Instance()->Type(filepath) == s2::SYM_IMAGE)
			{
				std::cout << i << " / " << n << " : " << filepath << "\n";
				EncodeByPvrTexTool(filepath);
			}
		}

	} 
	else if (ee::FileHelper::IsFileExist(path))
	{
		if (ee::SymbolFile::Instance()->Type(path) == s2::SYM_IMAGE) {
			EncodeByPvrTexTool(path);
		}
	}
}

void TransToPVR::EncodeByDtexPvr(const std::string& filepath) const
{
// 	ee::Image* img = ee::ImageMgr::Instance()->GetItem(filepath);
// 
// 	int w, h, c, f;
// 	uint8_t* src_buf = eimage::ImageIO::Read(filepath.c_str(), w, h, c, f);
// 
// 	// 			const uint8_t* src_buf = img->GetPixelData();
// 	// 			w = img->GetOriginWidth();
// 	// 			h = img->GetOriginHeight();
// 
// 	uint8_t* pvr_buf = dtex_pvr_encode(src_buf, w, h);
// 	delete[] src_buf;
// 
// 	std::string out_file = filepath.substr(0, filepath.find_last_of('.')) + ".pvr";
// 	dtex_pvr_write_file(out_file.c_str(), pvr_buf, w, h);
// 	free(pvr_buf);
}

void TransToPVR::EncodeByPvrTexTool(const std::string& filepath) const
{
	int w, h, c, f;
	uint8_t* pixels = ee::LibpngAdapter::Read(filepath.c_str(), w, h, c, f);
	eimage::TransToPVR trans(pixels, w, h, c);

	std::string out_file = filepath.substr(0, filepath.find_last_of('.')) + ".pvr";
	trans.OutputFile(out_file);
}

}