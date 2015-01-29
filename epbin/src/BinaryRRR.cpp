#include "BinaryRRR.h"
#include "ImageIDer.h"
#include "Lzma.h"

#include <easyimage.h>
#include <dtex_pvr.h>

namespace epbin
{

static const uint8_t TYPE = 13;

BinaryRRR::BinaryRRR(const std::vector<std::string>& src_files, const std::string& img_id_file)
{
	LoadPictures(src_files, img_id_file);
}

BinaryRRR::~BinaryRRR()
{
	for_each(m_pics.begin(), m_pics.end(), DeletePointerFunctor<Picture>());
}

void BinaryRRR::Pack(const std::string& outfile, bool compress) const
{
	int32_t pic_sz = m_pics.size();

	// data sz
	size_t data_sz = 0;
	data_sz += sizeof(int32_t);
	for (int i = 0; i < pic_sz; ++i) {
		data_sz += m_pics[i]->Size();
	}

	// fill buffer
	uint8_t* data_buf = new uint8_t[data_sz];
	uint8_t* ptr_data = data_buf;
	memcpy(ptr_data, &pic_sz, sizeof(pic_sz));
	ptr_data += sizeof(pic_sz);
	for (int i = 0; i < pic_sz; ++i) {
		m_pics[i]->Store(&ptr_data);
	}
	assert(ptr_data - data_buf == data_sz);

	// final
	size_t sz = data_sz + sizeof(uint8_t) + sizeof(uint32_t);
	uint8_t* buf = new uint8_t[sz];
	uint8_t* ptr = buf;
	memcpy(ptr, &TYPE, sizeof(uint8_t));
	ptr += sizeof(uint8_t);
	int cap = dtex_rrp_size(data_buf, data_sz);
	memcpy(ptr, &cap, sizeof(uint32_t));
	ptr += sizeof(uint32_t);
	memcpy(ptr, data_buf, data_sz);
	delete[] data_buf;

	// write to file
	std::ofstream fout(outfile.c_str(), std::ios::binary);
	if (compress)
	{
		uint8_t* dst = NULL;
		size_t dst_sz;
		Lzma::Compress(&dst, &dst_sz, buf, sz);

		fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(uint32_t));
		fout.write(reinterpret_cast<const char*>(dst), dst_sz);
	}
	else
	{
		int _sz = -(int)sz;
		fout.write(reinterpret_cast<const char*>(&_sz), sizeof(int32_t));
		fout.write(reinterpret_cast<const char*>(buf), sz);
	}
	delete[] buf;
	fout.close();
}

void BinaryRRR::LoadPictures(const std::vector<std::string>& src_files,
							 const std::string& img_id_file)
{
	ImageIDer ider(img_id_file);
	for (int i = 0, n = src_files.size(); i < n; ++i)
	{
		const std::string& filepath = src_files[i];

		Picture* pic = CreatePicture(filepath);
		pic->id = ider.Query(filepath);
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

		Part part(pic);
		part.x = rect.x / 4;
		part.y = rect.y / 4;
		part.w = rect.w / 4;
		part.h = rect.h / 4;
		pic->parts.push_back(part);
	}

	delete[] src_pixels;

	return pic;
}

//////////////////////////////////////////////////////////////////////////
// class BinaryRRR::Part
//////////////////////////////////////////////////////////////////////////

static const int BLOCK_SIZE = sizeof(int64_t);

size_t BinaryRRR::Part::
Size() const
{
	return sizeof(int16_t) * 4 + w * h * BLOCK_SIZE;
}

void BinaryRRR::Part::
Store(uint8_t** ptr)
{
	memcpy(*ptr, &x, sizeof(x));
	*ptr += sizeof(x);

	memcpy(*ptr, &y, sizeof(y));
	*ptr += sizeof(y);

	memcpy(*ptr, &w, sizeof(w));
	*ptr += sizeof(w);

	memcpy(*ptr, &h, sizeof(h));
	*ptr += sizeof(h);

	for (int iy = y; iy < y + h; ++iy) {
		for (int ix = x; ix < x + w; ++ix) {
			int idx = dtex_pvr_get_morton_number(ix, iy);
			int64_t* data = (int64_t*)pic->pixels + idx;
			memcpy(*ptr, data, sizeof(int64_t));
			*ptr += sizeof(int64_t);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// class BinaryRRR::Picture
//////////////////////////////////////////////////////////////////////////

size_t BinaryRRR::Picture::
Size() const
{
	size_t sz = 0;
	sz += sizeof(int16_t) * 4;
	for (int i = 0, n = parts.size(); i < n; ++i) {
		sz += parts[i].Size();
	}
	return sz;
}

void BinaryRRR::Picture::
Store(uint8_t** ptr)
{
	memcpy(*ptr, &id, sizeof(id));
	*ptr += sizeof(id);

	memcpy(*ptr, &w, sizeof(w));
	*ptr += sizeof(w);
	memcpy(*ptr, &h, sizeof(h));
	*ptr += sizeof(h);

	int16_t sz = parts.size();
	memcpy(*ptr, &sz, sizeof(sz));
	*ptr += sizeof(sz);
	for (int i = 0; i < sz; ++i) {
		parts[i].Store(ptr);
	}
}

}