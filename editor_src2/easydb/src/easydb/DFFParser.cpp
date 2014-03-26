#include "DFFParser.h"

#include <drag2d.h>

namespace edb
{

DFFParser::DFFParser(const char* filepath)
{
	int w, h, c, f;
	uint8_t* pixels = d2d::ImageLoader::loadData(filepath, w, h, c, f);
	assert(w == 32 && h == 32 && c == 4);
	m_alphas = new unsigned char[32*32];
	int ptr = 3;
	for (int i = 0, n = 32*32; i < n; ++i)
	{
		m_alphas[i] = pixels[ptr];
		ptr += 4;
	}
	delete[] pixels;

	m_dir = d2d::FilenameTools::getFileDir(filepath);
}

void DFFParser::outputImage(int width, int height)
{
	uint8_t* pixels = new uint8_t[width*height*4];
	int ptr = 0;
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			for (int k = 0; k < 3; ++k) {
				pixels[ptr++] = 0;
			}
			pixels[ptr++] = computeAlpha(width, height, i, height - 1 - j);
		}
	}

	std::string filepath = m_dir + "_" + wxString::FromDouble(width) + "_" + wxString::FromDouble(height);
	d2d::ImageSaver::storeToFile(pixels, width, height, filepath, d2d::ImageSaver::e_png);
	delete[] pixels;
}

inline float interpolate(int min, int max, float percent)
{
	return min + (max - min) * percent;
}

int DFFParser::computeAlpha(int width, int height, int x, int y)
{
	// todo width < 32

	int alpha;
	if (width == 32 && height == 32)
	{
		alpha = getAlpha(x, y);
	}
	else
	{
		int xlen = width / 32;
		int ylen = height / 32;
		int h_xlen = xlen >> 1;
		int h_ylen = ylen >> 1;

		int left, right, top, bottom;
		if (x < h_xlen) {
			left = right = 0;
		} else if (x >= width - h_xlen) {
			left = right = 31;
		} else {
			left = (x - h_xlen) / xlen;
			right = left + 1;
		}
		if (y < h_ylen) {
			top = bottom = 0;
		} else if (y >= height - h_ylen) {
			top = bottom = 31;
		} else {
			top = (y - h_ylen) / ylen;
			bottom = top + 1;
		}
		int left_top = getAlpha(left, top);
		int left_bottom = getAlpha(left, bottom);
		int right_bottom = getAlpha(right, bottom);
		int right_top = getAlpha(right, top);

		left = h_xlen + xlen * left;
		right = left + xlen;
		top = h_ylen + ylen * top;
		bottom = top + ylen;

		float top_val, bottom_val;
		if (x >= left && x <= right) {
			float percent = (x - left)/(float)xlen;
			assert(percent >= 0 && percent <= 1);
			top_val = interpolate(left_top, right_top, percent);
			bottom_val = interpolate(left_bottom, right_bottom, percent);
		} else if (x < left) {
			top_val = left_top;
			bottom_val = left_bottom;
		} else if (x > right) {
			top_val = right_top;
			bottom_val = right_bottom;
		}

		if (y >= top && y <= bottom) {
			float percent = (y - top)/(float)ylen;
			assert(percent >= 0 && percent <= 1);
			alpha = interpolate(top_val, bottom_val, percent);		
		} else if (y < top) {
			alpha = top_val;
		} else if (y > bottom) {
			alpha = bottom_val;
		}
	}

	if (alpha >= 128) {
 		//alpha = (alpha - 128) / 128.0f * 255.0f;

		float factor = 0.25f + 0.5f * alpha / 255.0f;
		alpha = 255 * factor;
 	} else {
 		alpha = 0;
 	}

	return alpha;
}

int DFFParser::getAlpha(int x, int y)
{
	assert(x >= 0 && x < 32 && y >= 0 && y < 32);
	return m_alphas[y * 32 + x];
}

}