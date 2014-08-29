#ifndef _DRAG2D_LABEL_NEW_H_
#define _DRAG2D_LABEL_NEW_H_

namespace d2d
{

class Screen;
class Vector;

class LabelNew
{
public:
	void Print(const Screen& screen, const char* text, const Vector& pos,
		int font_size, int width, int height) const;

}; // LabelNew

}

#endif // _DRAG2D_LABEL_NEW_H_