#include "TexturePacker.h"

namespace libcoco
{

TexturePacker::TexturePacker()
	: m_edge(512)
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

void TexturePacker::output(const std::string& floder, const std::string& filename)
{
	// ppm
	int ppm_size = 16 + m_edge * m_edge * 3;
	unsigned char* ppm_data = new unsigned char[ppm_size];
	sprintf((char*)ppm_data, 
		"P6\n"
		"%d %d\n"
		"%d\n"
		, m_edge, m_edge, 255);

	// pgm
	int pgm_size = 16 + m_edge * m_edge;
	unsigned char* pgm_data = new unsigned char[pgm_size];
	sprintf((char*)pgm_data, 
		"P5\n"
		"%d %d\n"
		"%d\n"
		, m_edge, m_edge, 15);

	std::map<d2d::Image*, d2d::Rect>::iterator itr 
		= m_mapImg2Rect.begin();
	int ppm_line_size = m_edge * 3;
	int pgm_line_size = m_edge;
	for ( ; itr != m_mapImg2Rect.end(); ++itr)
	{
		const unsigned char* pixels = itr->first->getPixelData();
		const d2d::Rect& r = itr->second;
		int src_line_size = r.xLength() * 4;
		for (int i = 0, n = r.yLength(); i < n ;++i)
		{
			int ptr_src = src_line_size * (r.yLength() - i - 1);
			int ptr_ppm = 16 + ppm_line_size * (r.yMin + i) + r.xMin * 3;
			int ptr_pgm = 16 + pgm_line_size * (r.yMin + i) + r.xMin;
			for (int j = 0, m = r.xLength(); j < m; ++j)
			{
// 				ppm_data[ptr_ppm++] = pixels[ptr_src++];
// 				ppm_data[ptr_ppm++] = pixels[ptr_src++];
// 				ppm_data[ptr_ppm++] = pixels[ptr_src++];
//				pgm_data[ptr_pgm++] = pixels[ptr_src++];

				unsigned char r = pixels[ptr_src++],
					g = pixels[ptr_src++],
					b = pixels[ptr_src++],
					a = pixels[ptr_src++];
				if (r != 0)
					int zz = 0;
				ppm_data[ptr_ppm++] = r / 16;
				ppm_data[ptr_ppm++] = g / 16;
				ppm_data[ptr_ppm++] = b / 16;
				pgm_data[ptr_pgm++] = a / 16;
			}
		}
	}

	// write to file
 	std::string ppm_path = floder + "\\" + filename + ".ppm";
 	std::ofstream ppm_fout(ppm_path.c_str(), std::ios::binary);
	ppm_fout.write(reinterpret_cast<const char*>(ppm_data), ppm_size);
	delete[] ppm_data;
 	ppm_fout.close();

	std::string pgm_path = floder + "\\" + filename + ".pgm";
	std::ofstream pgm_fout(pgm_path.c_str(), std::ios::binary);
	pgm_fout.write(reinterpret_cast<const char*>(pgm_data), pgm_size);
	delete[] pgm_data;
	pgm_fout.close();
}

}