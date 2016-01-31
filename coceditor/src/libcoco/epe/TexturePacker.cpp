#include "TexturePacker.h"

namespace libcoco
{
namespace epe
{

TexturePacker::TexturePacker(int padding, int extrude)
	: m_edge(768)
	, m_pixels(NULL)
	, m_padding(padding)
	, m_extrude(extrude)
{
	m_xCurr = m_yCurr = m_width = 0;
}

void TexturePacker::pack(const std::set<ee::Image*>& images)
{
	std::vector<ee::Image*> sorted;
	copy(images.begin(), images.end(), back_inserter(sorted));
	std::sort(sorted.begin(), sorted.end(), ImageCmp());

	int extra_half = m_padding + m_extrude;
	int extra_tot = (extra_half << 1);
	for (int i = 0, n = sorted.size(); i < n; ++i)
	{
		ee::Image* img = sorted[i];
		ee::Rect r;
		int w = img->GetClippedWidth() + extra_tot,
			h = img->GetClippedHeight() + extra_tot;
		assert(w < m_edge && h < m_edge);
		if (m_width == 0)
			m_width = w;
		if (m_yCurr + h > m_edge)
		{
			assert(m_xCurr + m_width + w < m_edge);
			m_xCurr += m_width;
			m_width = w;
			m_yCurr = 0;
		}
		r.xmin = m_xCurr;
		r.xmax = r.xmin + w;
		r.ymin = m_yCurr;
		r.ymax = r.ymin + h;
		m_yCurr += h;

		r.xmin += extra_half;
		r.xmax -= extra_half;
		r.ymin += extra_half;
		r.ymax -= extra_half;
		m_mapImg2Rect.insert(std::make_pair(img, r));
	}
}

void TexturePacker::storeToMemory()
{
	const int channels = 4;

	delete[] m_pixels;
	int size = m_edge * m_edge * channels;
	m_pixels = new uint8_t[size];
	memset(&m_pixels[0], 0, size);

	std::map<ee::Image*, ee::Rect>::iterator itr 
		= m_mapImg2Rect.begin();
	int size_line = m_edge * channels;
	for ( ; itr != m_mapImg2Rect.end(); ++itr)
	{
		const unsigned char* pixels = itr->first->GetPixelData();
		const ee::Rect& r = itr->second;
		//////////////////////////////////////////////////////////////////////////
		int src_line_size = r.Width() * channels;
		for (int i = 0, n = r.Height(); i < n ;++i)
		{
			int ptr_src = src_line_size * (r.Height() - i - 1);
			int ptr_dst = size_line * (r.ymin + i) + r.xmin * channels;
			memcpy(&m_pixels[ptr_dst], &pixels[ptr_src], src_line_size);
			for (int j = 0; j < m_extrude; ++j)
			{
				memcpy(&m_pixels[ptr_dst - channels * (j + 1)], &pixels[ptr_src], channels);
				memcpy(&m_pixels[ptr_dst + size_line + channels * j], &pixels[ptr_src + size_line - channels], channels);
			}
		}
		const int xoffset = (r.xmin - m_extrude) * channels;
		for (int i = 0; i < m_extrude; ++i)
		{
			int ptr_src = size_line * r.ymin + xoffset;
			int ptr_dst = size_line * (r.ymin - i - 1) + xoffset;
			memcpy(&m_pixels[ptr_dst], &m_pixels[ptr_src], channels * (r.Width() + 2 * m_extrude));
			ptr_src = size_line * (r.ymin + r.Height() - 1) + xoffset;
			ptr_dst = size_line * (r.ymin + r.Height() + i) + xoffset;
			memcpy(&m_pixels[ptr_dst], &m_pixels[ptr_src], channels * (r.Width() + 2 * m_extrude));
		}
	}
}

void TexturePacker::storeToFile(const std::string& floder, const std::string& filename, ee::ImageSaver::Type type)
{
	std::string filepath = floder + "\\" + filename;
	ee::ImageSaver::StoreToFile(m_pixels, m_edge, m_edge, 4, filepath, type);
}

const ee::Rect* TexturePacker::query(ee::Image* image) const
{
	std::map<ee::Image*, ee::Rect>::const_iterator itr 
		= m_mapImg2Rect.find(image);
	if (itr != m_mapImg2Rect.end())
		return &itr->second;
	else
		return NULL;
}

}
}