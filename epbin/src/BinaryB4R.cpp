#include "BinaryB4R.h"
#include "ImageIDer.h"
#include "Lzma.h"
#include "tools.h"

#include <easyimage.h>
#include <dtex_pvr.h>
#include <dtex_b4r.h>

namespace epbin
{

static const uint8_t TYPE = 14;

BinaryB4R::BinaryB4R(const std::vector<std::string>& src_files, const std::string& img_id_file)
{
	LoadPictures(src_files, img_id_file);
}

BinaryB4R::~BinaryB4R()
{
	for_each(m_pics.begin(), m_pics.end(), DeletePointerFunctor<Picture>());
}

void BinaryB4R::Pack(const std::string& outfile, bool compress) const
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
	int cap = dtex_b4r_size(data_buf, data_sz);
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

void BinaryB4R::LoadPictures(const std::vector<std::string>& src_files, const std::string& img_id_file)
{
	ImageIDer ider(img_id_file);
	for (int i = 0, n = src_files.size(); i < n; ++i)
	{
		wxString str;
		str.Printf("BinaryB4R load pic [%d/%d]\n", i, n);
		std::cout << str;

		const std::string& filepath = src_files[i];

		Picture* pic = CreatePicture(filepath);
		pic->id = ider.Query(filepath);
		m_pics.push_back(pic);
	}
}

BinaryB4R::Picture* BinaryB4R::CreatePicture(const std::string& filepath) const
{
	int sw, sh, sc, sf;
	uint8_t* src_pixels = eimage::ImageIO::Read(filepath.c_str(), sw, sh, sc, sf);

	// cut
	eimage::RegularRectCut cut(src_pixels, sw, sh);
	cut.AutoCut();

	// to pvr
	eimage::TransToPVR trans(src_pixels, sw, sh, sc);
	int w, h;
	uint8_t* pixels = trans.GetPixelsData(w, h);

	// create pic
	Picture* pic = new Picture;

	pic->bmp_w = sw;
	pic->bmp_h = sh;
	pic->bmp_pixels = src_pixels;

	pic->path = filepath;

	pic->w = w;
	pic->h = h;
	size_t sz = w * h * 0.5f;
	pic->pvr_pixels = new uint8_t[sz];
	memcpy(pic->pvr_pixels, pixels, sz);

	// flag data
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

	return pic;
}

//////////////////////////////////////////////////////////////////////////
// class BinaryB4R::Picture
//////////////////////////////////////////////////////////////////////////

BinaryB4R::Picture::
~Picture()
{
	delete[] bmp_pixels;
	delete[] pvr_pixels;
	delete[] flag;
}

size_t BinaryB4R::Picture::
Size() const
{
	size_t sz = 0;

	sz += sizeof(int16_t) * 3;

	sz += flag_sz;
	sz += block_used * sizeof(int64_t);

	return sz;
}

void BinaryB4R::Picture::
Store(uint8_t** ptr)
{
	memcpy(*ptr, &id, sizeof(id));
	*ptr += sizeof(id);

	memcpy(*ptr, &bmp_w, sizeof(bmp_w));
	*ptr += sizeof(bmp_w);
	memcpy(*ptr, &bmp_h, sizeof(bmp_h));
	*ptr += sizeof(bmp_h);

	memcpy(*ptr, flag, flag_sz);
	*ptr += flag_sz;

	int block = w >> 2;
	int i = 0;
	for (int y = 0; y < block; ++y) {
		for (int x = 0; x < block; ++x) {
			if (!IsBlockTransparent(x, y)) {
				assert(flag[i / 8] & (1 << (i % 8)));
				int idx = dtex_pvr_get_morton_number(x, y);
				int64_t* ptr_src = (int64_t*)pvr_pixels + idx;
				memcpy(*ptr, ptr_src, sizeof(int64_t));
				*ptr += sizeof(int64_t);
			}
			++i;
		}
	}
}

bool BinaryB4R::Picture::
IsBlockTransparent(int x, int y) const
{
	return IsBMPBlockTransparent(x, y);
}

bool BinaryB4R::Picture::
IsPVRBlockTransparent(int x, int y) const
{
	int idx = dtex_pvr_get_morton_number(x, y);
	int64_t* ptr = (int64_t*)pvr_pixels + idx;
	return *((int32_t*)ptr) == 0xaaaaaaaa;
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