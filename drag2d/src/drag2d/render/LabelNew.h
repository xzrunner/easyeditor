#ifndef _DRAG2D_LABEL_NEW_H_
#define _DRAG2D_LABEL_NEW_H_

#include <common/Color.h>
#include <common/types.h>

#include <vector>

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
	HoriAlignType align_hori;
	VertAlignType align_vert;
};

struct Glyph;

class LabelNew
{
public:
	void Print(const Screen& screen, const char* text, const Vector& pos,
		const LabelStyle& style) const;

private:
	struct Line
	{
		std::vector<const Glyph*> glyphs;
		int width, height;

		Line() {
			width = height = 0;
		}

		void Clear() {
			glyphs.clear();
			width = height = 0;
		}
	}; 

}; // LabelNew

}

#endif // _DRAG2D_LABEL_NEW_H_