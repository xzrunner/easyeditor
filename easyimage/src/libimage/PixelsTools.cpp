#include "PixelsTools.h"

namespace eimage
{

byte* RGBA2RGB(const byte* rgba, int width, int height, bool invert_y)
{
	int size = width * height;
	byte* rgb = new byte[size * 3];
	if (invert_y)
	{
		int ptr = 0;
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				int p = ((height - 1 - i) * width + j) * 4;
				rgb[ptr++] = rgba[p++];
				rgb[ptr++] = rgba[p++];
				rgb[ptr++] = rgba[p++];
			}
		}
	}
	else
	{
		int ptr_src = 0;
		int ptr_dst = 0;
		for (int i = 0; i < size; ++i)
		{
			rgb[ptr_dst++] = rgba[ptr_src++];
			rgb[ptr_dst++] = rgba[ptr_src++];
			rgb[ptr_dst++] = rgba[ptr_src++];
			ptr_src++;
		}
	}
	return rgb;
}

}