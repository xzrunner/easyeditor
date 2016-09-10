#include "ImageLoader.h"

namespace gum
{

void ImageLoader::FormatPixelsAlpha(uint8_t* pixels, int width, int height, int val)
{
	int ptr = 0;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			uint8_t r = pixels[ptr],
					g = pixels[ptr+1],
					b = pixels[ptr+2],
					a = pixels[ptr+3];
			if (a == 0) {
				r = g = b = val;
			}
			pixels[ptr++] = r;
			pixels[ptr++] = g;
			pixels[ptr++] = b;
			pixels[ptr++] = a;
		}
	}
}

void ImageLoader::PreMuiltiAlpha(uint8_t* pixels, int width, int height)
{
	int pos = 0;
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			float alpha = pixels[pos + 3] / 255.0f;
			for (int i = 0; i < 3; ++i) {
				pixels[pos + i] = static_cast<uint8_t>(pixels[pos + i] * alpha);
			}
			pos += 4;
		}
	}
}

void ImageLoader::RemoveGhostPixel(uint8_t* pixels, int width, int height)
{
	int ptr = 0;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			uint8_t r = pixels[ptr],
					g = pixels[ptr+1],
					b = pixels[ptr+2],
					a = pixels[ptr+3];

// 			if (r <= 3 && g <= 3 && b <= 3 && a <= 3) {
// 				r = g = b = a = 0;
// 			}
			if (a <= 3) {
				a = 0;
			}

			pixels[ptr++] = r;
			pixels[ptr++] = g;
			pixels[ptr++] = b;
			pixels[ptr++] = a;
		}
	}
}

}