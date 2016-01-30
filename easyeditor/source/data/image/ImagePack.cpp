#include "ImagePack.h"

#include <assert.h>

namespace ee
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
						 PackType type, bool bpp4, int extrude_left, int extrude_bottom, int extrude_right, int extrude_up)
{
	assert(dst_x >= 0 && dst_y >= 0);
	if (type == PT_NORMAL) {
		assert(dst_x + src_w <= m_width && dst_y + src_h <= m_height);
		for (int iy = 0; iy < src_h; ++iy) {
			for (int ix = 0; ix < src_w; ++ix) {
				CopyPixel(src_buf, src_w, src_h, bpp4, ix, iy, dst_x + ix, dst_y + iy);
			}
		}
		// left
		for (int i = 0; i < extrude_left; ++i) {
			for (int y = 0; y < src_h; ++y) {
				CopyPixel(src_buf, src_w, src_h, bpp4, 0, y, dst_x - i - 1, dst_y + y);
			}
		}
		// right
		for (int i = 0; i < extrude_right; ++i) {
			for (int y = 0; y < src_h; ++y) {
				CopyPixel(src_buf, src_w, src_h, bpp4, src_w - 1, y, dst_x + src_w + i, dst_y + y);
			}
		}
		// bottom
		for (int i = 0; i < extrude_bottom; ++i) {
			for (int x = 0; x < src_w; ++x) {
				CopyPixel(src_buf, src_w, src_h, bpp4, x, 0, dst_x + x, dst_y - i - 1);
			}
		}
		// up
		for (int i = 0; i < extrude_up; ++i) {
			for (int x = 0; x < src_w; ++x) {
				CopyPixel(src_buf, src_w, src_h, bpp4, x, src_h - 1, dst_x + x, dst_y + src_h + i);
			}
		}
		// left-bottom
		for (int x = 0; x < extrude_left; ++x) {
			for (int y = 0; y < extrude_bottom; ++y) {
				CopyPixel(src_buf, src_w, src_h, bpp4, 0, 0, dst_x - x - 1, dst_y - y - 1);
			}
		}
		// right-bottom
		for (int x = 0; x < extrude_right; ++x) {
			for (int y = 0; y < extrude_bottom; ++y) {
				CopyPixel(src_buf, src_w, src_h, bpp4, src_w - 1, 0, dst_x + src_w + x, dst_y - y - 1);
			}
		}
		// left-up
		for (int x = 0; x < extrude_left; ++x) {
			for (int y = 0; y < extrude_up; ++y) {
				CopyPixel(src_buf, src_w, src_h, bpp4, 0, src_h - 1, dst_x - x - 1, dst_y + src_h + y);
			}
		}
		// right-up
		for (int x = 0; x < extrude_right; ++x) {
			for (int y = 0; y < extrude_up; ++y) {
				CopyPixel(src_buf, src_w, src_h, bpp4, src_w - 1, src_h - 1, dst_x + src_w + x, dst_y + src_h + y);
			}
		}
	} else {
		assert(dst_x + src_h <= m_width && dst_y + src_w <= m_height);
		if (type == PT_CLOCKWISE) {
			for (int iy = 0; iy < src_h; ++iy) {
				for (int ix = 0; ix < src_w; ++ix) {
					CopyPixelClockwise(src_buf, src_w, src_h, bpp4, ix, iy, dst_x + src_h - 1 - iy, dst_y + ix);
				}
			}
			// left
			for (int i = 0; i < extrude_left; ++i) {
				for (int y = 0; y < src_h; ++y) {
					CopyPixel(src_buf, src_w, src_h, bpp4, 0, y, dst_x + src_h - 1 - y, dst_y - i - 1);
				}
			}
			// right
			for (int i = 0; i < extrude_right; ++i) {
				for (int y = 0; y < src_h; ++y) {
					CopyPixel(src_buf, src_w, src_h, bpp4, src_w - 1, y, dst_x + src_h - 1 - y, dst_y + src_w + i);
				}
			}
			// bottom
 			for (int i = 0; i < extrude_bottom; ++i) {
 				for (int x = 0; x < src_w; ++x) {
 					CopyPixel(src_buf, src_w, src_h, bpp4, x, 0, dst_x + src_h + i, dst_y + x);
 				}
 			}
			// up
			for (int i = 0; i < extrude_up; ++i) {
				for (int x = 0; x < src_w; ++x) {
					CopyPixel(src_buf, src_w, src_h, bpp4, x, src_h - 1, dst_x - i - 1, dst_y + x);
				}
			}
			// left-bottom
			for (int x = 0; x < extrude_bottom; ++x) {
				for (int y = 0; y < extrude_left; ++y) {
					CopyPixel(src_buf, src_w, src_h, bpp4, 0, 0, dst_x + src_h + x, dst_y - y - 1);
				}
			}
			// right-bottom
			for (int x = 0; x < extrude_bottom; ++x) {
				for (int y = 0; y < extrude_right; ++y) {
					CopyPixel(src_buf, src_w, src_h, bpp4, src_w - 1, 0, dst_x + src_h + x, dst_y + src_w + y);
				}
			}
			// left-up
			for (int x = 0; x < extrude_up; ++x) {
				for (int y = 0; y < extrude_left; ++y) {
					CopyPixel(src_buf, src_w, src_h, bpp4, 0, src_h - 1, dst_x - x - 1, dst_y - y - 1);
				}
			}
			// right-up
			for (int x = 0; x < extrude_up; ++x) {
				for (int y = 0; y < extrude_right; ++y) {
					CopyPixel(src_buf, src_w, src_h, bpp4, src_w - 1, src_h - 1, dst_x - x - 1, dst_y + src_w + y);
				}
			}
		} else if (type == PT_ANTICLOCKWISE) {
			for (int iy = 0; iy < src_h; ++iy) {
				for (int ix = 0; ix < src_w; ++ix) {
					CopyPixelAntiClockwise(src_buf, src_w, src_h, bpp4, ix, iy, dst_x + iy, dst_y + src_w - 1 - ix);
				}
			}
			// left
			for (int i = 0; i < extrude_left; ++i) {
				for (int y = 0; y < src_h; ++y) {
					CopyPixel(src_buf, src_w, src_h, bpp4, 0, y, dst_x + y, dst_y + src_w + i);					
				}
			}
			// right
			for (int i = 0; i < extrude_right; ++i) {
				for (int y = 0; y < src_h; ++y) {
					CopyPixel(src_buf, src_w, src_h, bpp4, src_w - 1, y, dst_x + y, dst_y - i - 1);
				}
			}
			// bottom
 			for (int i = 0; i < extrude_bottom; ++i) {
 				for (int x = 0; x < src_w; ++x) {
 					CopyPixel(src_buf, src_w, src_h, bpp4, x, 0, dst_x - i - 1, dst_y + src_w - 1 - x);
 				}
 			}
			// up
			for (int i = 0; i < extrude_up; ++i) {
				for (int x = 0; x < src_w; ++x) {
					CopyPixel(src_buf, src_w, src_h, bpp4, x, src_h - 1, dst_x + src_h + i, dst_y + src_w - 1 - x);
				}
			}
			// left-bottom
			for (int x = 0; x < extrude_bottom; ++x) {
				for (int y = 0; y < extrude_left; ++y) {
					CopyPixel(src_buf, src_w, src_h, bpp4, 0, 0, dst_x - x - 1, dst_y + src_w + y);
				}
			}
			// right-bottom
			for (int x = 0; x < extrude_bottom; ++x) {
				for (int y = 0; y < extrude_right; ++y) {
					CopyPixel(src_buf, src_w, src_h, bpp4, src_w - 1, 0, dst_x - x - 1, dst_y - y - 1);
				}
			}
			// left-up
			for (int x = 0; x < extrude_up; ++x) {
				for (int y = 0; y < extrude_left; ++y) {
					CopyPixel(src_buf, src_w, src_h, bpp4, 0, src_h - 1, dst_x + src_h + x, dst_y + src_w + y);
				}
			}
			// right-up
			for (int x = 0; x < extrude_up; ++x) {
				for (int y = 0; y < extrude_right; ++y) {
					CopyPixel(src_buf, src_w, src_h, bpp4, src_w - 1, src_h - 1, dst_x + src_h + x, dst_y - y - 1);
				}
			}
		}
	}
}

void ImagePack::AddImage(const Image* img, int x, int y, int w, int h, bool rotate, bool clockwise, 
						 bool bpp4, int extrude_left, int extrude_bottom, int extrude_right, int extrude_up)
{
	int sw = img->GetOriginWidth(),
		sh = img->GetOriginHeight();
	PackType type = PT_NORMAL;
	if (rotate) {
		type = clockwise ? PT_CLOCKWISE : PT_ANTICLOCKWISE;
	}
	AddImage(img->GetPixelData(), sw, sh, x, y, type, bpp4, extrude_left, extrude_bottom, extrude_right, extrude_up);
}

void ImagePack::OutputToFile(const std::string& filepath) const
{
	LibpngAdapter::Write(m_pixels, m_width, m_height, filepath.c_str(), false);
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