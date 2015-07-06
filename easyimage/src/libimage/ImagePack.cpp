#include "ImagePack.h"
#include "ImageIO.h"

namespace eimage
{

static const int BPP3 = 3;
static const int BPP4 = 4;

ImagePack::ImagePack(int width, int height)
	: m_width(width)
	, m_height(height)
{
	int sz = width * height * BPP4;
	m_pixels = new uint8_t[sz];
	memset(m_pixels, 0, sizeof(uint8_t) * sz);
}

ImagePack::~ImagePack()
{
	delete[] m_pixels;
}

void ImagePack::AddImage(const uint8_t* src_buf, int src_w, int src_h, int dst_x, int dst_y, 
						 PackType type, bool bpp4, int extrude)
{
	assert(dst_x >= 0 && dst_y >= 0);
	if (type == PT_NORMAL) {
		assert(dst_x + src_w <= m_width && dst_y + src_h <= m_height);
		for (int iy = 0; iy < src_h; ++iy) {
			for (int ix = 0; ix < src_w; ++ix) {
				CopyPixel(src_buf, src_w, src_h, bpp4, ix, iy, dst_x + ix, dst_y + iy);
			}
		}
		// extrude
		for (int i = 0; i < extrude; ++i)
		{
			for (int y = 0; y < src_h; ++y) {
				CopyPixel(src_buf, src_w, src_h, bpp4, 0, y, dst_x - i - 1, dst_y + y);		// left
				CopyPixel(src_buf, src_w, src_h, bpp4, src_w - 1, y, dst_x + src_w + i, dst_y + y);		// right
			}
			for (int x = 0; x < src_w; ++x) {
				CopyPixel(src_buf, src_w, src_h, bpp4, x, 0, dst_x + x, dst_y - i - 1);		// bottom
				CopyPixel(src_buf, src_w, src_h, bpp4, x, src_h - 1, dst_x + x, dst_y + src_h + i);		// up
			}
		}
	} else {
// 		assert(dst_x + src_h <= m_width && dst_y + src_w <= m_height);
// 		if (type == PT_CLOCKWISE) {
// 			for (int iy = 0; iy < src_w; ++iy) {
// 				for (int ix = 0; ix < src_h; ++ix) {
// 					int ptr_src = ((src_w - 1 - iy) * src_h + ix) * BYTES_PER_PIXEL,
// 						ptr_dst = ((dst_y + ix) * m_width + (dst_x + src_w - 1 - iy)) * BYTES_PER_PIXEL;
// 					for (int i = 0; i < BYTES_PER_PIXEL; ++i) { 
// 						assert(ptr_dst < m_width * m_height * BYTES_PER_PIXEL);
// 						m_pixels[ptr_dst++] = src_buf[ptr_src++];
// 					}
// 				}
// 			}
// 		} else if (type == PT_ANTICLOCKWISE) {
// 			for (int iy = 0; iy < src_w; ++iy) {
// 				for (int ix = 0; ix < src_h; ++ix) {
// 					int ptr_src = ((src_w - 1 - iy) * src_h + ix) * BYTES_PER_PIXEL,
// 						ptr_dst = ((dst_y + src_h - 1 - ix) * m_width + (dst_x + iy)) * BYTES_PER_PIXEL;
// 					for (int i = 0; i < BYTES_PER_PIXEL; ++i) { 
// 						assert(ptr_dst < m_width * m_height * BYTES_PER_PIXEL);
// 						m_pixels[ptr_dst++] = src_buf[ptr_src++];
// 					}
// 				}
// 			}
// 		}

		//////////////////////////////////////////////////////////////////////////

		assert(dst_x + src_h <= m_width && dst_y + src_w <= m_height);
		if (type == PT_CLOCKWISE) {
			for (int iy = 0; iy < src_h; ++iy) {
				for (int ix = 0; ix < src_w; ++ix) {
					CopyPixelClockwise(src_buf, src_w, src_h, bpp4, ix, iy, dst_x + src_h - 1 - iy, dst_y + ix);
				}
			}
			// extrude
			for (int i = 0; i < extrude; ++i)
			{
				for (int y = 0; y < src_h; ++y) {
					CopyPixel(src_buf, src_w, src_h, bpp4, 0, y, dst_x + src_h - 1 - y, dst_y - i - 1);
					CopyPixel(src_buf, src_w, src_h, bpp4, src_w - 1, y, dst_x + src_h - 1 - y, dst_y + src_w + i);
				}
				for (int x = 0; x < src_w; ++x) {
					CopyPixel(src_buf, src_w, src_h, bpp4, x, 0, dst_x + src_h + i, dst_y + x);
					CopyPixel(src_buf, src_w, src_h, bpp4, x, src_h - 1, dst_x - i - 1, dst_y + x);
				}
			}
		} else if (type == PT_ANTICLOCKWISE) {
			for (int iy = 0; iy < src_h; ++iy) {
				for (int ix = 0; ix < src_w; ++ix) {
					CopyPixelAntiClockwise(src_buf, src_w, src_h, bpp4, ix, iy, dst_x + iy, dst_y + src_w - 1 - ix);
				}
			}
			// extrude
			for (int i = 0; i < extrude; ++i)
			{
				for (int y = 0; y < src_h; ++y) {
					CopyPixel(src_buf, src_w, src_h, bpp4, 0, y, dst_x + y, dst_y + src_w + i);
					CopyPixel(src_buf, src_w, src_h, bpp4, src_w - 1, y, dst_x + y, dst_y - i - 1);
				}
				for (int x = 0; x < src_w; ++x) {
					CopyPixel(src_buf, src_w, src_h, bpp4, x, 0, dst_x - i - 1, dst_y + src_w - 1 - x);
					CopyPixel(src_buf, src_w, src_h, bpp4, x, src_h - 1, dst_x + src_h + i, dst_y + src_w - 1 - x);
				}
			}
		}
	}
}

void ImagePack::AddImage(const d2d::Image* img, int x, int y, int w, int h, bool clockwise, 
						 bool bpp4, int extrude)
{
	int sw = img->GetOriginWidth(),
		sh = img->GetOriginHeight();
	PackType type = PT_NORMAL;
	if (sw == w && sh == h) {
		type = PT_NORMAL;
	} else if (sw == h && sh == w) {
		type = clockwise ? PT_CLOCKWISE : PT_ANTICLOCKWISE;
	} else {
		assert(0);
	}
	AddImage(img->GetPixelData(), sw, sh, x, y, type, bpp4, extrude);
}

void ImagePack::PreMuiltiAlpha()
{
	int pos = 0;
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			float alpha = m_pixels[pos + 3] / 255.0f;
			for (int i = 0; i < 3; ++i) {
				m_pixels[pos + i] = m_pixels[pos + i] * alpha;
			}
			pos += 4;
		}
	}
}

void ImagePack::OutputToFile(const wxString& filepath) const
{
	ImageIO::Write(m_pixels, m_width, m_height, filepath.mb_str());
}

void ImagePack::CopyPixel(const uint8_t* src, int sw, int sh, bool sbpp4,
						  int sx, int sy, int dx, int dy)
{
	int ptr_dst = (dy * m_width + dx) * BPP4;
	if (sbpp4) 
	{
		int ptr_src = ((sh - 1 - sy) * sw + sx) * BPP4;
		for (int i = 0; i < BPP4; ++i) {
			assert(ptr_dst < m_width * m_height * BPP4);
			m_pixels[ptr_dst++] = src[ptr_src++];
		}
	} 
	else 
	{
		int ptr_src = ((sh - 1 - sy) * sw + sx) * BPP3;
		for (int i = 0; i < BPP3; ++i) {
			assert(ptr_dst < m_width * m_height * BPP4);
			m_pixels[ptr_dst++] = src[ptr_src++];
		}
		m_pixels[ptr_dst++] = 255;
	}
}

void ImagePack::CopyPixelClockwise(const uint8_t* src, int sw, int sh, bool sbpp4, 
								   int sx, int sy, int dx, int dy)
{
	int ptr_dst = (dy * m_width + dx) * BPP4;
	if (sbpp4) {
		int ptr_src = ((sh - 1 - sy) * sw + sx) * BPP4;
		for (int i = 0; i < BPP4; ++i) { 
			assert(ptr_dst < m_width * m_height * BPP4);
			m_pixels[ptr_dst++] = src[ptr_src++];
		}
	} else {
		int ptr_src = ((sh - 1 - sy) * sw + sx) * BPP3;
		for (int i = 0; i < BPP4 - 1; ++i) {
			assert(ptr_dst < m_width * m_height * BPP4);
			m_pixels[ptr_dst++] = src[ptr_src++];
		}
		m_pixels[ptr_dst++] = 255;
	}
}

void ImagePack::CopyPixelAntiClockwise(const uint8_t* src, int sw, int sh, bool sbpp4, 
									   int sx, int sy, int dx, int dy)
{
	int ptr_dst = (dy * m_width + dx) * BPP4;
	if (sbpp4) {
		int ptr_src = ((sh - 1 - sy) * sw + sx) * BPP4;
		for (int i = 0; i < BPP4; ++i) { 
			assert(ptr_dst < m_width * m_height * BPP4);
			m_pixels[ptr_dst++] = src[ptr_src++];
		}
	} else {
		int ptr_src = ((sh - 1 - sy) * sw + sx) * BPP3;
		for (int i = 0; i < BPP4 - 1; ++i) {
			assert(ptr_dst < m_width * m_height * BPP4);
			m_pixels[ptr_dst++] = src[ptr_src++];
		}
		m_pixels[ptr_dst++] = 255;
	}
}

}