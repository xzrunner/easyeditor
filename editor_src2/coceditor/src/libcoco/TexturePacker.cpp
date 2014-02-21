#include "TexturePacker.h"

namespace libcoco
{

TexturePacker::TexturePacker()
	: m_edge(512)
	, m_pixels(NULL)
{
	m_xCurr = m_yCurr = m_width = 0;
}

void TexturePacker::pack(const std::set<d2d::Image*>& images)
{
	std::vector<d2d::Image*> sorted;
	copy(images.begin(), images.end(), back_inserter(sorted));
	std::sort(sorted.begin(), sorted.end(), ImageCmp());

	for (int i = 0, n = sorted.size(); i < n; ++i)
	{
		d2d::Image* img = sorted[i];
		d2d::Rect r;
		int w = img->width(),
			h = img->height();
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
		r.xMin = m_xCurr;
		r.xMax = r.xMin + w;
		r.yMin = m_yCurr;
		r.yMax = r.yMin + h;
		m_yCurr += h;

		m_mapImg2Rect.insert(std::make_pair(img, r));
	}
}

void TexturePacker::storeToMemory()
{
	delete[] m_pixels;
	int size = m_edge * m_edge * 4;
	m_pixels = new uint8_t[size];

	std::map<d2d::Image*, d2d::Rect>::iterator itr 
		= m_mapImg2Rect.begin();
	int size_line = m_edge * 4;
	for ( ; itr != m_mapImg2Rect.end(); ++itr)
	{
		const unsigned char* pixels = itr->first->getPixelData();
		const d2d::Rect& r = itr->second;
		int src_line_size = r.xLength() * 4;
		for (int i = 0, n = r.yLength(); i < n ;++i)
		{
			int ptr_src = src_line_size * (r.yLength() - i - 1);
			int ptr_dst = 16 + size_line * (r.yMin + i) + r.xMin * 4;
			memcpy(&m_pixels[ptr_dst], &pixels[ptr_src], src_line_size);
		}
	}
}

void TexturePacker::storeToFile(const std::string& floder, const std::string& filename, d2d::ImageSaver::Type type)
{
	std::string filepath = floder + "\\" + filename;
	d2d::ImageSaver::storeToFile(m_pixels, m_edge, m_edge, filepath, type);
}

const d2d::Rect* TexturePacker::query(d2d::Image* image) const
{
	std::map<d2d::Image*, d2d::Rect>::const_iterator itr 
		= m_mapImg2Rect.find(image);
	if (itr != m_mapImg2Rect.end())
		return &itr->second;
	else
		return NULL;
}

}