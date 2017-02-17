#include "TexturePacker.h"

#include <ee/cfg_const.h>
#include <ee/ImageData.h>

#include <vector>
#include <algorithm>
#include <string>

#include <gimg_typedef.h>
#include <gimg_export.h>

#include <assert.h>

namespace ecoco
{
namespace epe
{

TexturePacker::TexturePacker(int padding, int extrude)
	: m_edge(ee::SCREEN_HEIGHT)
	, m_pixels(NULL)
	, m_padding(padding)
	, m_extrude(extrude)
{
	m_xcurr = m_ycurr = m_width = 0;
}

void TexturePacker::Pack(const std::set<ee::Image*>& images)
{
	std::vector<ee::Image*> sorted;
	copy(images.begin(), images.end(), back_inserter(sorted));
	std::sort(sorted.begin(), sorted.end(), ImageCmp());

	int extra_half = m_padding + m_extrude;
	int extra_tot = (extra_half << 1);
	for (int i = 0, n = sorted.size(); i < n; ++i)
	{
		ee::Image* img = sorted[i];
		sm::rect r;
		int w = img->GetClippedRegion().Width() + extra_tot,
			h = img->GetClippedRegion().Height() + extra_tot;
		assert(w < m_edge && h < m_edge);
		if (m_width == 0)
			m_width = w;
		if (m_ycurr + h > m_edge)
		{
			assert(m_xcurr + m_width + w < m_edge);
			m_xcurr += m_width;
			m_width = w;
			m_ycurr = 0;
		}
		r.xmin = static_cast<float>(m_xcurr);
		r.xmax = r.xmin + w;
		r.ymin = static_cast<float>(m_ycurr);
		r.ymax = r.ymin + h;
		m_ycurr += h;

		r.xmin += extra_half;
		r.xmax -= extra_half;
		r.ymin += extra_half;
		r.ymax -= extra_half;
		m_map_img2rect.insert(std::make_pair(img, r));
	}
}

void TexturePacker::StoreToMemory()
{
	const int channels = 4;

	delete[] m_pixels;
	int size = m_edge * m_edge * channels;
	m_pixels = new uint8_t[size];
	memset(&m_pixels[0], 0, size);

	std::map<ee::Image*, sm::rect>::iterator itr 
		= m_map_img2rect.begin();
	int size_line = m_edge * channels;
	for ( ; itr != m_map_img2rect.end(); ++itr)
	{
		ee::ImageData* img_data = ee::ImageDataMgr::Instance()->GetItem(itr->first->GetFilepath());
		const unsigned char* pixels = img_data->GetPixelData();
		const sm::rect& r = itr->second;
		sm::vec2 sz = r.Size();
		//////////////////////////////////////////////////////////////////////////
		int src_line_size = static_cast<int>(sz.x * channels);
		for (int i = 0, n = static_cast<int>(sz.y); i < n ;++i)
		{
			int ptr_src = src_line_size * (static_cast<int>(sz.y) - i - 1);
			int ptr_dst = static_cast<int>(size_line * (r.ymin + i) + r.xmin * channels);
			memcpy(&m_pixels[ptr_dst], &pixels[ptr_src], src_line_size);
			for (int j = 0; j < m_extrude; ++j)
			{
				memcpy(&m_pixels[ptr_dst - channels * (j + 1)], &pixels[ptr_src], channels);
				memcpy(&m_pixels[ptr_dst + size_line + channels * j], &pixels[ptr_src + size_line - channels], channels);
			}
		}
		img_data->RemoveReference();

		const int xoffset = static_cast<int>((r.xmin - m_extrude) * channels);
		for (int i = 0; i < m_extrude; ++i)
		{
			int ptr_src = size_line * r.ymin + xoffset;
			int ptr_dst = size_line * (r.ymin - i - 1) + xoffset;
			memcpy(&m_pixels[ptr_dst], &m_pixels[ptr_src], channels * (sz.x + 2 * m_extrude));
			ptr_src = size_line * (r.ymin + sz.y - 1) + xoffset;
			ptr_dst = size_line * (r.ymin + sz.y + i) + xoffset;
			memcpy(&m_pixels[ptr_dst], &m_pixels[ptr_src], channels * (sz.x + 2 * m_extrude));
		}
	}
}

void TexturePacker::StoreToFile(const std::string& floder, const std::string& filename)
{
	std::string filepath = floder + "\\" + filename + ".png";
	gimg_export(filepath.c_str(), m_pixels, m_edge, m_edge, GPF_RGBA, true);
}

const sm::rect* TexturePacker::Query(ee::Image* image) const
{
	std::map<ee::Image*, sm::rect>::const_iterator itr 
		= m_map_img2rect.find(image);
	if (itr != m_map_img2rect.end())
		return &itr->second;
	else
		return NULL;
}

}
}