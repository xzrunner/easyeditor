#include "BinaryB4R.h"
#include "ImageIDer.h"
#include "Lzma.h"
#include "tools.h"

#include <ee/LibpngAdapter.h>
#include <ee/StringHelper.h>

#include <easyimage.h>
// #include <dtex_pvr.h>
// #include <dtex_b4r.h>

#include <algorithm>

#include <wx/string.h>

namespace epbin
{

static const uint8_t TYPE = 14;

static const uint16_t TEX_PVR = 1;
static const uint16_t TEX_ETC1 = 2;

BinaryB4R::BinaryB4R(const std::vector<std::string>& src_files, 
					 const std::string& img_id_file,
					 bool is_pvr)
	: m_is_pvr(is_pvr)
{
	LoadPictures(src_files, img_id_file);
}

BinaryB4R::~BinaryB4R()
{
	for_each(m_pics.begin(), m_pics.end(), DeletePointerFunctor<Picture>());
}

void BinaryB4R::Pack(const std::string& outfile, bool compress) const
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
// 	int cap = dtex_b4r_size(data_buf, data_sz);
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

void BinaryB4R::LoadPictures(const std::vector<std::string>& src_files, const std::string& img_id_file)
{
	ImageIDer ider(img_id_file);
	for (int i = 0, n = src_files.size(); i < n; ++i)
	{
		std::cout << ee::StringHelper::Format("BinaryB4R load pic [%d/%d]\n", i, n);

		const std::string& filepath = src_files[i];

		Picture* pic = CreatePicture(filepath);
		pic->id = ider.Query(filepath);
		m_pics.push_back(pic);
	}
}

BinaryB4R::Picture* BinaryB4R::CreatePicture(const std::string& filepath) const
{
	int sw, sh, sc, sf;
	uint8_t* src_pixels = ee::LibpngAdapter::Read(filepath.c_str(), sw, sh, sc, sf);

// 	// cut
// 	eimage::RegularRectCut cut(src_pixels, sw, sh);
// 	cut.AutoCut();

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
	pic->bmp_pixels = src_pixels;

	pic->path = filepath;

	pic->w = w;
	pic->h = h;
	size_t sz = m_is_pvr ? w * h * 0.5f : w * h;
	pic->compressed_pixels = new uint8_t[sz];
	memcpy(pic->compressed_pixels, pixels, sz);

	// flag data
	if (m_is_pvr) {
		assert(w == h && w % 4 == 0);
		int block = w >> 2;
		int block_sz = block * block;

		pic->flag_sz = std::ceil(block_sz / 8.0f);
		pic->flag = new uint8_t[pic->flag_sz];
		pic->block_used = 0;
		memset(pic->flag, 0, pic->flag_sz);
		int i = 0;
		for (int y = 0; y < block; ++y) {
			for (int x = 0; x < block; ++x) {
				if (!pic->IsBlockTransparent(x, y)) {
					pic->flag[i / 8] |= (1 << (i % 8));
					++pic->block_used;
				}
				++i;
			}
		}
	} else {
		assert(w % 4 == 0 && h % 4 == 0);
		int bw = w >> 2,
			bh = h >> 2;
		int block_sz = bw * bh;
		pic->flag_sz = std::ceil(block_sz / 8.0f);
		pic->flag = new uint8_t[pic->flag_sz];
		pic->block_used = 0;
		memset(pic->flag, 0, pic->flag_sz);
		int i = 0;
		for (int y = 0; y < bh; ++y) {
			for (int x = 0; x < bw; ++x) {
				if (!pic->IsBlockTransparent(x, y)) {
					pic->flag[i / 8] |= (1 << (i % 8));
					++pic->block_used;
				}
				++i;
			}
		}
	}

	return pic;
}

//////////////////////////////////////////////////////////////////////////
// class BinaryB4R::Picture
//////////////////////////////////////////////////////////////////////////

size_t BinaryB4R::Picture::
BlockSize(bool is_pvr)
{
	return is_pvr ? sizeof(int64_t) : sizeof(int64_t) * 2;
}

BinaryB4R::Picture::
~Picture()
{
	delete[] bmp_pixels;
	delete[] compressed_pixels;
	delete[] flag;
}

size_t BinaryB4R::Picture::
Size(bool is_pvr) const
{
	size_t sz = 0;

	sz += sizeof(int16_t) * 3;

	sz += flag_sz;
	sz += block_used * BlockSize(is_pvr);

	return sz;
}

void BinaryB4R::Picture::
Store(bool is_pvr, uint8_t** ptr)
{
// 	pack2mem(id, ptr);
// 
// 	pack2mem(bmp_w, ptr);
// 	pack2mem(bmp_h, ptr);
// 
// 	memcpy(*ptr, flag, flag_sz);
// 	*ptr += flag_sz;
// 
// 	if (is_pvr) {
// 		int block = w >> 2;
// 		int i = 0;
// 		for (int y = 0; y < block; ++y) {
// 			for (int x = 0; x < block; ++x) {
// 				if (!IsBlockTransparent(x, y)) {
// 					assert(flag[i / 8] & (1 << (i % 8)));
// 					int idx = dtex_pvr_get_morton_number(x, y);
// 					int64_t* ptr_src = (int64_t*)compressed_pixels + idx;
// 					pack2mem(*ptr_src, ptr);
// 				}
// 				++i;
// 			}
// 		}
// 	} else {
// 		int bw = w >> 2,
// 			bh = h >> 2;
// 		int block_count = bw * bh;
// 		int i = 0;
// 		for (int y = 0; y < bh; ++y) {
// 			for (int x = 0; x < bw; ++x) {
// 				if (!IsBlockTransparent(x, y)) {
// 					assert(flag[i / 8] & (1 << (i % 8)));
// 					int idx = y * bw + x;
// 
// 					int64_t* rgb_data = (int64_t*)compressed_pixels + idx;
// 					pack2mem(*rgb_data, ptr);
// 
// 					int64_t* alpha_data = (int64_t*)compressed_pixels + block_count + idx;
// 					pack2mem(*alpha_data, ptr);
// 				}
// 				++i;
// 			}
// 		}
// 	}
}

bool BinaryB4R::Picture::
IsBlockTransparent(int x, int y) const
{
	return IsBMPBlockTransparent(x, y);
}

bool BinaryB4R::Picture::
IsPVRBlockTransparent(int x, int y) const
{
// 	int idx = dtex_pvr_get_morton_number(x, y);
// 	int64_t* ptr = (int64_t*)compressed_pixels + idx;
// 	return *((int32_t*)ptr) == 0xaaaaaaaa;

	return false;
}

bool BinaryB4R::Picture::
IsBMPBlockTransparent(int x, int y) const
{
	for (int px = x * 4; px < x * 4 + 4; ++px) {
		for (int py = y * 4; py < y * 4 + 4; ++py) {
			if (px >= bmp_w || py >= bmp_h) {
				continue;
			}

			int _py = bmp_h - 1 - py;
			if (bmp_pixels[(bmp_w * _py + px) * 4 + 4 - 1] != 0) {
				return false;
			}
		}
	}

	return true;
}

}