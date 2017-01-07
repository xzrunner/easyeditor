#include "PackPNG.h"
#include "Lzma.h"
#include "typedef.h"

#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/Exception.h>
#include <ee/ImageData.h>
#include <ee/Image.h>
#include <ee/ImageSymbol.h>
#include <ee/ImageVerticalFlip.h>

#include <easyimage.h>

#include <gimg_import.h>
#include <gimg_typedef.h>
#include <sprite2/DrawRT.h>

#include <assert.h>

namespace erespacker
{

PackPNG::PackPNG(bool png8)
	: m_width(0)
	, m_height(0)
	, m_buffer(NULL)
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

	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	bool ori_clip_cfg = data.open_image_edge_clip;
	data.open_image_edge_clip = false;
	bool ori_alpha_cfg = data.pre_multi_alpha;
	data.pre_multi_alpha = false;

	int w, h, fmt;
	uint8_t* buf = gimg_import(filepath.c_str(), &w, &h, &fmt);
	assert(fmt == GPF_RGB || fmt == GPF_RGBA);
	int c = fmt == GPF_RGB ? 3 : 4;

	data.open_image_edge_clip = ori_clip_cfg;
	data.pre_multi_alpha = ori_alpha_cfg;

	m_width = w;
	m_height = h;
	if (c == 3) {
		m_buffer = new uint8_t[w * h * 4];
		uint8_t *src = buf, *dst = m_buffer;
		for (int i = 0, n = w * h; i < n; ++i) {
			memcpy(dst, src, sizeof(uint8_t) * 3);
			src += 3;
			dst += 3;
			*dst = 255;
			dst += 1;
		}
	} else if (c == 4) {
		m_buffer = buf;
	} else {
		throw ee::Exception("PackPNG::Load: error channels %d.", c);
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
		StoreScaled(fout, scale);
	}
	fout.close();
}

void PackPNG::StoreScaled(std::ofstream& fout, float scale) const
{
	size_t sz = m_width * m_height * 4;
	uint8_t* buf = new uint8_t[sz];
	memcpy(buf, m_buffer, sz);

	ee::ImageData img_data(buf, m_width, m_height, 4);
	ee::Image img(&img_data);
	ee::ImageSymbol sym(&img, "");

	int width = static_cast<int>(m_width * scale),
		height= static_cast<int>(m_height * scale);

	s2::DrawRT rt;
	rt.Draw(&sym, false, scale);
	uint8_t* buffer = rt.StoreToMemory(width, height);

	ee::ImageVerticalFlip revert(buffer, width, height);
	uint8_t* buf_revert = revert.Revert();		
	Store(fout, buf_revert, width, height);

	delete[] buf_revert;
	delete[] buffer;
}

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