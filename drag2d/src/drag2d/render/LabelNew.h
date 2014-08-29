#ifndef _DRAG2D_LABEL_NEW_H_
#define _DRAG2D_LABEL_NEW_H_

#include <common/Color.h>

namespace d2d
{

class Screen;
class Vector;

struct LabelStyle
{
	int font_size;
	int width, height;
	Colorf color;
	bool has_edge;
};

class LabelNew
{
public:
	void Print(const Screen& screen, const char* text, const Vector& pos,
		const LabelStyle& style) const;

}; // LabelNew

}

#endif // _DRAG2D_LABEL_NEW_H_