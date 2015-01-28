#include "BinaryRRR.h"

#include <easyimage.h>

namespace epbin
{

BinaryRRR::BinaryRRR(const std::vector<std::string>& src_files, const std::string& img_id_file)
{
	
}

BinaryRRR::~BinaryRRR()
{

}

void BinaryRRR::Pack(const std::string& outfile, bool compress) const
{
	
}

void BinaryRRR::LoadPictures(const std::vector<std::string>& src_files)
{
	for (int i = 0, n = src_files.size(); i < n; ++i)
	{
		const std::string& filepath = src_files[i];

		Picture* pic = CreatePicture(filepath);
		m_pics.push_back(pic);
	}
}

BinaryRRR::Picture* BinaryRRR::CreatePicture(const std::string& filepath) const
{
	int sw, sh, sc, sf;
	uint8_t* src_pixels = eimage::ImageIO::Read(filepath.c_str(), sw, sh, sc, sf);

	// cut
	eimage::RegularRectCut cut(src_pixels, sw, sh);
	cut.AutoCut();

	// to pvr
	eimage::TransToPVR trans(src_pixels, sw, sh, sc);
	int w, h;
	uint8_t* pixels = trans.GetPVRData(w, h);

	// create pic
	Picture* pic = new Picture;

	pic->path = filepath;

	pic->w = w;
	pic->h = h;
	size_t sz = w * h * 0.5f;
	pic->pixels = new uint8_t[sz];
	memcpy(pic->pixels, pixels, sz);

	const std::vector<eimage::Rect>& rects = cut.GetResult();
	for (int i = 0, n = rects.size(); i < n; ++i)
	{
		const eimage::Rect& rect = rects[i];

		Part part;
		part.x = rect.x / 4;
		part.y = rect.y / 4;
		part.w = rect.w / 4;
		part.h = rect.h / 4;
		pic->parts.push_back(part);
	}

	delete[] src_pixels;

	return pic;
}

}