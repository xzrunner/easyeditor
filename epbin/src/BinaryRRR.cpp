#include "BinaryRRR.h"
#include "ImageIDer.h"
#include "Lzma.h"
#include "tools.h"

#include <easyimage.h>
// #include <dtex_pvr.h>
// #include <dtex_rrr.h>

namespace epbin
{

static const uint8_t TYPE = 13;

static const uint16_t TEX_PVR = 1;
static const uint16_t TEX_ETC1 = 2;

BinaryRRR::BinaryRRR(const std::vector<std::string>& src_files, 
					 const std::string& img_id_file,
					 bool is_pvr)
	: m_is_pvr(is_pvr)
{
	LoadPictures(src_files, img_id_file);
}

BinaryRRR::~BinaryRRR()
{
	for_each(m_pics.begin(), m_pics.end(), DeletePointerFunctor<Picture>());
}

void BinaryRRR::Pack(const std::string& outfile, bool compress) const
{
// 	uint16_t pic_sz = m_pics.size();
// 
// 	// data sz
// 	size_t data_sz = 0;
// 	data_sz += sizeof(int32_t);
// 	for (int i = 0; i < pic_sz; ++i) {
// 		data_sz += m_pics[i]->Size(m_is_pvr);
// 	}
// 
// 	// fill buffer
// 	uint8_t* data_buf = new uint8_t[data_sz];
// 	uint8_t* ptr_data = data_buf;
// 	// store pic_sz
// 	pack2mem(pic_sz, &ptr_data);
// 	// store tex type
// 	uint16_t type = m_is_pvr ? TEX_PVR : TEX_ETC1;
// 	pack2mem(type, &ptr_data);
// 	// store pictures
// 	for (int i = 0; i < pic_sz; ++i) {
// 		m_pics[i]->Store(m_is_pvr, &ptr_data);
// 	}
// 	assert(ptr_data - data_buf == data_sz);
// 
// 	// final
// 	size_t sz = data_sz + sizeof(uint8_t) + sizeof(uint32_t);
// 	uint8_t* buf = new uint8_t[sz];
// 	uint8_t* ptr = buf;
// 	pack2mem(TYPE, &ptr);
// 
// 	int cap = dtex_rrr_size(data_buf, data_sz);
// 	pack2mem(cap, &ptr);
// 
// 	memcpy(ptr, data_buf, data_sz);
// 	delete[] data_buf;
// 
// 	// write to file
// 	std::ofstream fout(outfile.c_str(), std::ios::binary);
// 	if (compress)
// 	{
// 		uint8_t* dst = NULL;
// 		size_t dst_sz;
// 		Lzma::Compress(&dst, &dst_sz, buf, sz);
// 
// 		fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(uint32_t));
// 		fout.write(reinterpret_cast<const char*>(dst), dst_sz);
// 	}
// 	else
// 	{
// 		int _sz = -(int)sz;
// 		fout.write(reinterpret_cast<const char*>(&_sz), sizeof(int32_t));
// 		fout.write(reinterpret_cast<const char*>(buf), sz);
// 	}
// 	delete[] buf;
// 	fout.close();
}

void BinaryRRR::LoadPictures(const std::vector<std::string>& src_files,
							 const std::string& img_id_file)
{
	ImageIDer ider(img_id_file);
	for (int i = 0, n = src_files.size(); i < n; ++i)
	{
		wxString str;
		str.Printf("BinaryRRR load pic [%d/%d]\n", i, n);
		std::cout << str;

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

	// compress texture
	uint8_t* pixels;
	int w, h;
	if (m_is_pvr) {
		eimage::TransToPVR trans(src_pixels, sw, sh, sc);
		pixels = trans.GetPixelsData(w, h);
	} else {
		eimage::TransToETC1 trans(src_pixels, sw, sh, sc);
		pixels = trans.GetPixelsData(w, h);
	}

	// create pic
	Picture* pic = new Picture;

	pic->bmp_w = sw;
	pic->bmp_h = sh;

	pic->path = filepath;

	pic->w = w;
	pic->h = h;
	size_t sz = w * h * 0.5f;
	pic->pixels = pixels;

	const std::vector<eimage::Rect>& rects = cut.GetResult();
	for (int i = 0, n = rects.size(); i < n; ++i)
	{
		const eimage::Rect& rect = rects[i];

		Part part(pic);
		part.x = rect.x / 4;
		part.y = (sh - rect.y - rect.h) / 4;
//		assert(part.y >= 0);
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

size_t BinaryRRR::Part::BlockSize(bool is_pvr)
{
	return is_pvr ? sizeof(int64_t) : sizeof(int64_t) * 2;
}

size_t BinaryRRR::Part::
Size(bool is_pvr) const
{
//	return sizeof(int16_t) * 4 + w * h * BlockSize(is_pvr);

	int _w = w, _h = h;
	if (x < 0) {
		_w = w + x;
		assert(_w > 0);
	}
	if (y < 0) {
		_h = h + y;
		assert(_h > 0);
	}
	return sizeof(int16_t) * 4 + _w * _h * BlockSize(is_pvr);
}

void BinaryRRR::Part::
Store(bool is_pvr, uint8_t** ptr)
{
// 	pack2mem(x, ptr);
// 	pack2mem(y, ptr);
// 	pack2mem(w, ptr);
// 	pack2mem(h, ptr);
// 
// 	size_t block_sz = BlockSize(is_pvr);
// 	int bw = pic->w >> 2,
// 		bh = pic->h >> 2;
// 	int block_count = bw * bh;
// 	for (int iy = y; iy < y + h; ++iy) {
// 		for (int ix = x; ix < x + w; ++ix) {
// 			if (ix < 0 || iy < 0) {
// 				continue;
// 			}
// 
// 			if (is_pvr) {
// 				int idx = dtex_pvr_get_morton_number(ix, iy);
// 				int64_t* data = (int64_t*)pic->pixels + idx;
// 				memcpy(*ptr, data, block_sz);
// 				*ptr += block_sz;
// 			} else {
// 				int idx = iy * bw + ix;
// 
// 				int64_t* rgb_data = (int64_t*)pic->pixels + idx;
// 				pack2mem(*rgb_data, ptr);
// 
// 				int64_t* alpha_data = (int64_t*)pic->pixels + (block_count + idx);
// 				pack2mem(*alpha_data, ptr);
// 			}
// 		}
// 	}
}

//////////////////////////////////////////////////////////////////////////
// class BinaryRRR::Picture
//////////////////////////////////////////////////////////////////////////

size_t BinaryRRR::Picture::
Size(bool is_pvr) const
{
	int block_count = 0;
	
	size_t sz = 0;
	sz += sizeof(int16_t) * 4;
	for (int i = 0, n = parts.size(); i < n; ++i) {
		sz += parts[i].Size(is_pvr);

		block_count += parts[i].h * parts[i].w;
	}
	return sz;
}

void BinaryRRR::Picture::
Store(bool is_pvr, uint8_t** ptr)
{
	pack2mem(id, ptr);

	pack2mem(bmp_w, ptr);
	pack2mem(bmp_h, ptr);

	int16_t sz = parts.size();
	pack2mem(sz, ptr);
	for (int i = 0; i < sz; ++i) {
		parts[i].Store(is_pvr, ptr);
	}
}

}