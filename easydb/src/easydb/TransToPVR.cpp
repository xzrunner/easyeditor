#include "TransToPVR.h"
#include "check_params.h"

#include <drag2d.h>
#include <easyimage.h>
#include <dtex_pvr.h>

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
	return Command() + " [dir path]";
}

void TransToPVR::Run(int argc, char *argv[])
{
	// png2pvr e:/test2/1001

	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);
}

void TransToPVR::Trigger(const std::string& dir)
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
			std::cout << i << " / " << n << " : " << filepath << "\n";
			EncodeByDtexPvr(filepath);
		}
	}
}

void TransToPVR::EncodeByDtexPvr(const wxString& filepath) const
{
	d2d::Image* img = d2d::ImageMgr::Instance()->getItem(filepath);

	int w, h, c, f;
	uint8_t* src_buf = eimage::ImageIO::Read(filepath.c_str(), w, h, c, f);

	// 			const uint8_t* src_buf = img->getPixelData();
	// 			w = img->originWidth();
	// 			h = img->originHeight();

	uint8_t* pvr_buf = dtex_pvr_encode(src_buf, w, h);
	delete[] src_buf;

	std::string out_file = filepath.substr(0, filepath.find_last_of('.')) + ".pvr";
	dtex_pvr_write_file(out_file.c_str(), pvr_buf, w, h);
	free(pvr_buf);
}

void TransToPVR::EncodeByPvrTexTool(const wxString& filepath) const
{
	eimage::TransToPVR::Trans(filepath.ToStdString());
}

}