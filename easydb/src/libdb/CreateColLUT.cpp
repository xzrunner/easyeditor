#include "CreateColLUT.h"
#include "check_params.h"
#include "utility.h"

#include <gimg_png.h>
#include <gimg_typedef.h>

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
	int grid = 16;
	int s = 1;
	int w = (grid + 2) * 16 * s,
		h = (grid + 2) * s;
	uint8_t* pixels = new uint8_t[w * h * 4];
	memset(pixels, 0xff, w * h * 4);
	for (int r = 0; r < grid; ++r) {
		for (int g = 0; g < grid; ++g) {
			for (int b = 0; b < 16; ++b) {
				int x = (b * grid + r + b * 2 + 1) * s,
					y = (g + 1) * s;
				for (int ix = 0; ix < s; ++ix) {
					for (int iy = 0; iy < s; ++iy) {
						DrawPixel(pixels, grid, w, h, x + ix, y + iy, r, g, b);
					}
				}
			}
		}
	}
	
	gimg_png_write(filepath.c_str(), pixels, w, h, GPF_RGBA, true);
	
	delete[] pixels;
}

void CreateColLUT::DrawPixel(uint8_t* pixels, int grid, int w, int h, int x, int y, int r, int g, int b)
{
	PixelTo256(&pixels[(y * w + x) * 4], grid, r, g, b);
	// col
	if (r == 0) {
		PixelTo256(&pixels[(y * w + x - 1) * 4], grid, r, g, b);
	} else if (r == grid - 1) {
		PixelTo256(&pixels[(y * w + x + 1) * 4], grid, r, g, b);
	}
	// row
	if (g == 0) {
		PixelTo256(&pixels[((y - 1) * w + x) * 4], grid, r, g, b);
	} else if (g == grid - 1) {
		PixelTo256(&pixels[((y + 1) * w + x) * 4], grid, r, g, b);
	}
	// corner
	if ((r == 0 || r == grid - 1) && (g == 0 || g == grid - 1)) {
		if (r == 0) --x;
		else ++x;
		if (g == 0) --y;
		else ++y;
		PixelTo256(&pixels[(y * w + x) * 4], grid, r, g, b);
	}
}

void CreateColLUT::PixelTo256(uint8_t* dst, int grid, int r, int g, int b)
{
	dst[0] = Int16To256(r, grid);
	dst[1] = Int16To256(g, grid);
	dst[2] = Int16To256(b, 16);
	dst[3] = 255;
}

uint8_t CreateColLUT::Int16To256(int i16, int grid)
{
	return static_cast<uint8_t>(i16 * 255.0f / (grid - 1) + 0.5f);

// 	i16 = (int)(i16 / 11) * 11;
// 	return static_cast<uint8_t>(i16 * 255.0f / 15.0f + 0.5f);
}

}