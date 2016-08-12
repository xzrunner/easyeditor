#include "PackETC2.h"
#include "Lzma.h"
#include "typedef.h"

#include <ee/Exception.h>
#include <ee/Symbol.h>
#include <ee/SymbolMgr.h>
#include <ee/Snapshoot.h>
#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/ImageVerticalFlip.h>

#include <easyimage.h>

#include <string>

#include <assert.h>

namespace erespacker
{

struct PKMHeader
{
	unsigned char identifier[8];
	unsigned char paddedWidthMSB;
	unsigned char paddedWidthLSB;
	unsigned char paddedHeightMSB;
	unsigned char paddedHeightLSB;
	unsigned char widthMSB;
	unsigned char widthLSB;
	unsigned char heightMSB;
	unsigned char heightLSB;
};

PackETC2::PackETC2(bool fast)
	: m_fast(fast)
{
	m_type = TT_ETC2;

	m_width = m_height = 0;
	m_buf = NULL;
}

PackETC2::~PackETC2()
{
	Clear();
}

void PackETC2::Load(const std::string& filepath)
{
	Clear();

	m_base_path = filepath.substr(0, filepath.find_last_of('.')) + ".png";

	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));

	PKMHeader header;
	fin.read(reinterpret_cast<char*>(&header), sizeof(header));

	m_width = (header.paddedWidthMSB << 8) | header.paddedWidthLSB;
	m_height = (header.paddedHeightMSB << 8) | header.paddedHeightLSB;
	uint32_t sz = m_width * m_height;
	m_buf = new uint8_t[sz];
	if (!m_buf) {
		throw ee::Exception("Out of memory: PackETC2::Load %s \n", filepath.c_str());
	}
	fin.read(reinterpret_cast<char*>(m_buf), sz);

	fin.close();
}

void PackETC2::Store(const std::string& filepath, float scale) const
{
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));
	if (scale == 1) {
		Store(fout, m_buf, m_width, m_height);
	} else {
		StoreScaled(fout, scale);
	}
	fout.close();
}

void PackETC2::Clear()
{
	delete m_buf, m_buf = NULL;
}

void PackETC2::Store(std::ofstream& fout, uint8_t* buffer, int width, int height) const
{
	if (m_compress)
	{
		size_t body_sz = width * height;
		size_t tot_size = sizeof(int8_t) + sizeof(int16_t) * 2 + body_sz;
		uint8_t* buf = new uint8_t[tot_size];
		uint8_t* ptr = buf;

		memcpy(ptr, &m_type, sizeof(int8_t));
		ptr += sizeof(int8_t);
		memcpy(ptr, &width, sizeof(int16_t));
		ptr += sizeof(int16_t);
		memcpy(ptr, &height, sizeof(int16_t));
		ptr += sizeof(int16_t);
		memcpy(ptr, buffer, body_sz);
		ptr += body_sz;

		uint8_t* dst = NULL;
		size_t dst_sz;
		Lzma::Compress(&dst, &dst_sz, buf, tot_size);

		delete[] buf;

		if (dst_sz > 0) {
			fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(uint32_t));
			fout.write(reinterpret_cast<const char*>(dst), dst_sz);
			delete[] dst;
		}
	}
	else
	{
		int body_sz = width * height;
		int sz = 0;
		sz += sizeof(int8_t);	// type
		sz += sizeof(int16_t);	// width
		sz += sizeof(int16_t);	// height
		sz += body_sz;
		sz = -sz;
		fout.write(reinterpret_cast<const char*>(&sz), sizeof(int32_t));

		fout.write(reinterpret_cast<const char*>(&m_type), sizeof(int8_t));
		fout.write(reinterpret_cast<const char*>(&width), sizeof(int16_t));
		fout.write(reinterpret_cast<const char*>(&height), sizeof(int16_t));

		fout.write(reinterpret_cast<const char*>(buffer), body_sz);
	}

}

void PackETC2::StoreScaled(std::ofstream& fout, float scale) const
{
	ee::SettingData& sd = ee::Config::Instance()->GetSettings();
	bool old_clip = sd.open_image_edge_clip;
	bool old_premul = sd.pre_multi_alpha;
	sd.open_image_edge_clip = false;
	sd.pre_multi_alpha = false;

	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(m_base_path);
	int w = static_cast<int>(m_width * scale),
		h = static_cast<int>(m_height * scale);
	ee::Snapshoot ss;
	uint8_t* png_buf = ss.OutputToMemory(sym, false, scale);
	sym->RemoveReference();

	ee::ImageVerticalFlip revert(png_buf, w, h);
	uint8_t* buf_revert = revert.Revert();		
	delete[] png_buf;

	eimage::TransToETC2 trans(buf_revert, w, h, 4, eimage::TransToETC2::RGBA, false, m_fast);
	delete[] buf_revert;

	uint8_t* etc2_buf = trans.GetPixelsData(w, h);
	Store(fout, etc2_buf, w, h);
	delete[] etc2_buf;

	sd.open_image_edge_clip = old_clip;
	sd.pre_multi_alpha = old_premul;
}

}