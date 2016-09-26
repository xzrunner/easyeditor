#include "CreateColLUT.h"
#include "check_params.h"
#include "utility.h"

#include <gimg_png.h>

#include <stdint.h>

namespace edb
{

std::string CreateColLUT::Command() const
{
	return "col-lut";
}

std::string CreateColLUT::Description() const
{
	return "create color LUT";
}

std::string CreateColLUT::Usage() const
{
	// col-lut E:\test2\image
	return Command() + " [dst path]";
}

int CreateColLUT::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;

	Run(argv[2]);

	return 0;
}

void CreateColLUT::Run(const std::string& filepath) const
{
	int w = 16 * 16 + 2 * 16,
		h = 18;
	uint8_t* pixels = new uint8_t[w * h * 4];
	memset(pixels, 0xff, w * h * 4);
	for (int r = 0; r < 16; ++r) {
		for (int g = 0; g < 16; ++g) {
			for (int b = 0; b < 16; ++b) {
				int x = b * 16 + r + b * 2 + 1,
					y = g + 1;
				Pixel16To256(&pixels[(y * w + x) * 4], r, g, b);
				// col
				if (r == 0) {
					Pixel16To256(&pixels[(y * w + x - 1) * 4], r, g, b);
				} else if (r == 15) {
					Pixel16To256(&pixels[(y * w + x + 1) * 4], r, g, b);
				}
				// row
				if (g == 0) {
					Pixel16To256(&pixels[((y - 1) * w + x) * 4], r, g, b);
				} else if (g == 15) {
					Pixel16To256(&pixels[((y + 1) * w + x) * 4], r, g, b);
				}
				// corner
				if ((r == 0 || r == 15) && (g == 0 || g == 15)) {
					if (r == 0) --x;
					else ++x;
					if (g == 0) --y;
					else ++y;
					Pixel16To256(&pixels[(y * w + x) * 4], r, g, b);
				}
			}
		}
	}
	
	gimg_png_write(filepath.c_str(), pixels, w, h);
	
	delete[] pixels;
}

void CreateColLUT::Pixel16To256(uint8_t* dst, int r, int g, int b)
{
	dst[0] = Int16To256(r);
	dst[1] = Int16To256(g);
	dst[2] = Int16To256(b);
	dst[3] = 255;
}

uint8_t CreateColLUT::Int16To256(int i16)
{
	return static_cast<uint8_t>(i16 * 255.0f / 15.0f + 0.5f);

// 	i16 = (int)(i16 / 11) * 11;
// 	return static_cast<uint8_t>(i16 * 255.0f / 15.0f + 0.5f);
}

}