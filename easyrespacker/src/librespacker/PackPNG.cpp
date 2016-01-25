#include "PackPNG.h"
#include "Lzma.h"
#include "typedef.h"

#include <drag2d.h>
#include <easyimage.h>

namespace librespacker
{

PackPNG::PackPNG(bool png8)
	: m_buffer(NULL)
{
	m_type = png8 ? TT_PNG8 : TT_PNG4;
}

PackPNG::~PackPNG()
{
	Clear();
}

void PackPNG::Load(const std::string& filepath)
{
	Clear();

	d2d::SettingData& data = d2d::Config::Instance()->GetSettings();
	bool ori_clip_cfg = data.open_image_edge_clip;
	data.open_image_edge_clip = false;
	bool ori_alpha_cfg = data.pre_multi_alpha;
	data.pre_multi_alpha = false;

	int w, h, c, f;
	uint8_t* buf = d2d::ImageLoader::FileToPixels(filepath, w, h, c, f);

	data.open_image_edge_clip = ori_clip_cfg;
	data.pre_multi_alpha = ori_alpha_cfg;

	m_width = w;
	m_height = h;
	m_buffer = buf;
	if (c != 4) {
		throw d2d::Exception("PackPNG::Load: image is not rgba.");
	}

	if (m_type == TT_PNG4) {
		GenPng4();
	}
}

void PackPNG::Store(const std::string& filepath, float scale) const
{
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));

	if (scale == 1) {
		Store(fout, m_buffer, m_width, m_height);
	} else {
		size_t sz = m_width * m_height * 4;
		uint8_t* buf = new uint8_t[sz];
		memcpy(buf, m_buffer, sz);

		d2d::ImageData img_data(buf, m_width, m_height, 4);
		d2d::Image img(&img_data);
		d2d::ImageSymbol symbol(&img, "");

		int width = m_width * scale,
			height = m_height * scale;
		d2d::Snapshoot ss;
		uint8_t* buffer = ss.OutputToMemory(&symbol, false, scale);
		Store(fout, buffer, width, height);
		delete[] buffer;
	}

	fout.close();
}

//void PackPNG::RevertAndStore(std::ofstream& fout, uint8_t* buffer, int width, int height) const
//{
//	eimage::ImageVerticalFlip revert(buffer, width, height);
//	uint8_t* buf_revert = revert.Revert();		
//	Store(fout, buf_revert, width, height);
//	delete[] buf_revert;
//}

void PackPNG::Clear()
{
	delete[] m_buffer, m_buffer = NULL;
}

static inline int
_round(int c) {
	c = (c + 7 > 0xff) ? (0xff) : (c + 7); 
	return c >> 4;
}

// todo
void PackPNG::GenPng4()
{
	int size = m_width * m_height;
	for (int i = 0; i < size; i += 4)
	{
		int r = _round(m_buffer[i]);  
		int g = _round(m_buffer[i+1]);
		int b = _round(m_buffer[i+2]);
		int a = _round(m_buffer[i+3]);
	}
}

void PackPNG::Store(std::ofstream& fout, uint8_t* buffer, 
					int width, int height) const
{
	int buf_sz = width * height * 4;
	if (m_compress)
	{
		size_t sz = sizeof(int8_t) + sizeof(int16_t) * 2 + buf_sz;
		uint8_t* buf = new uint8_t[sz];
		uint8_t* ptr = buf;

		memcpy(ptr, &m_type, sizeof(int8_t));
		ptr += sizeof(int8_t);
		memcpy(ptr, &width, sizeof(int16_t));
		ptr += sizeof(int16_t);
		memcpy(ptr, &height, sizeof(int16_t));
		ptr += sizeof(int16_t);
		memcpy(ptr, buffer, buf_sz);
		ptr += buf_sz;

		uint8_t* dst = NULL;
		size_t dst_sz;
		Lzma::Compress(&dst, &dst_sz, buf, sz);
		delete[] buf;

		if (dst_sz > 0) {
			fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(uint32_t));
			fout.write(reinterpret_cast<const char*>(dst), dst_sz);
			delete[] dst;
		}
	}
	else
	{
		int sz = 0;
		sz += sizeof(int8_t);	// type
		sz += sizeof(int16_t);	// width
		sz += sizeof(int16_t);	// height
		sz += buf_sz;			// buf
		sz = -sz;
		fout.write(reinterpret_cast<const char*>(&sz), sizeof(int32_t));

		fout.write(reinterpret_cast<const char*>(&m_type), sizeof(int8_t));
		fout.write(reinterpret_cast<const char*>(&width), sizeof(int16_t));
		fout.write(reinterpret_cast<const char*>(&height), sizeof(int16_t));
		fout.write(reinterpret_cast<const char*>(buffer), buf_sz);
	}
}

}