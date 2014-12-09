#ifndef _RT_PIXEL_H_
#define _RT_PIXEL_H_

namespace rt
{

class Pixel
{
public:
	Pixel(int x, int y, int red, int green, int blue);

public:
	int x, y;
	int red, green, blue;
};

inline Pixel::Pixel(int _x, int _y, int _red, int _green, int _blue)
	: x(_x), y(_y), red(_red), green(_green), blue(_blue)
{}

}

#endif // _RT_PIXEL_H_