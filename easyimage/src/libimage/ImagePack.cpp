#include "ImagePack.h"
#include "ImageIO.h"

namespace eimage
{

static const int BYTES_PER_PIXEL = 4;

ImagePack::ImagePack(int width, int height)
	: m_width(width)
	, m_height(height)
{
	int sz = width * height * BYTES_PER_PIXEL;
	m_pixels = new uint8_t[sz];
	memset(m_pixels, 0, sizeof(uint8_t) * sz);
}

ImagePack::~ImagePack()
{
	delete[] m_pixels;
}

void ImagePack::AddImage(const uint8_t* src_buf, int src_w, int src_h, int dst_x, int dst_y, PackType type)
{
	assert(dst_x >= 0 && dst_y >= 0);
	if (type == PT_NORMAL) {
		assert(dst_x + src_w <= m_width && dst_y + src_h <= m_height);
		for (int iy = 0; iy < src_h; ++iy) {
			for (int ix = 0; ix < src_w; ++ix) {
				int ptr_src = ((src_h - 1 - iy) * src_w + ix) * BYTES_PER_PIXEL,
					ptr_dst = ((dst_y + iy) * m_width + (dst_x + ix)) * BYTES_PER_PIXEL;
				for (int i = 0; i < BYTES_PER_PIXEL; ++i) {
					assert(ptr_dst < m_width * m_height * BYTES_PER_PIXEL);
					m_pixels[ptr_dst++] = src_buf[ptr_src++];
				}
			}
		}
	} else {
		assert(dst_x + src_h <= m_width && dst_y + src_w <= m_height);
		if (type == PT_CLOCKWISE) {
			for (int iy = 0; iy < src_w; ++iy) {
				for (int ix = 0; ix < src_h; ++ix) {
					int ptr_src = ((src_w - 1 - iy) * src_h + ix) * BYTES_PER_PIXEL,
						ptr_dst = ((dst_y + ix) * m_width + (dst_x + src_w - 1 - iy)) * BYTES_PER_PIXEL;
					for (int i = 0; i < BYTES_PER_PIXEL; ++i) { 
						assert(ptr_dst < m_width * m_height * BYTES_PER_PIXEL);
						m_pixels[ptr_dst++] = src_buf[ptr_src++];
					}
				}
			}
		} else if (type == PT_ANTICLOCKWISE) {
			for (int iy = 0; iy < src_w; ++iy) {
				for (int ix = 0; ix < src_h; ++ix) {
					int ptr_src = ((src_w - 1 - iy) * src_h + ix) * BYTES_PER_PIXEL,
						ptr_dst = ((dst_y + src_h - 1 - ix) * m_width + (dst_x + iy)) * BYTES_PER_PIXEL;
					for (int i = 0; i < BYTES_PER_PIXEL; ++i) { 
						assert(ptr_dst < m_width * m_height * BYTES_PER_PIXEL);
						m_pixels[ptr_dst++] = src_buf[ptr_src++];
					}
				}
			}
		}
	}
}

void ImagePack::AddImage(const d2d::Image* img, int x, int y, int w, int h, bool clockwise)
{
	int sw = img->originWidth(),
		sh = img->originHeight();
	PackType type = PT_NORMAL;
	if (sw == w && sh == h) {
		type = PT_NORMAL;
	} else if (sw == h && sh == w) {
		type = clockwise ? PT_CLOCKWISE : PT_ANTICLOCKWISE;
	} else {
		assert(0);
	}
	
	AddImage(img->getPixelData(), img->originWidth(), img->originHeight(), x, y, type);
}

void ImagePack::OutputToFile(const wxString& filepath) const
{
	ImageIO::Write(m_pixels, m_width, m_height, filepath.mb_str());
}

}