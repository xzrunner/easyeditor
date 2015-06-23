#ifndef _DRAG2D_LABEL_NEW_H_
#define _DRAG2D_LABEL_NEW_H_

#include "LabelLayout.h"

#include <common/Color.h>
#include <common/types.h>

#include <vector>
#include <wx/string.h>

namespace d2d
{

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
	static void Print(
		const char* text, 
		const Vector& pos,
		float scale,
		const LabelStyle& style);

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
	}; // Line

private:
	static void TransToUnicodes(
		const char* text, 
		std::vector<int>& unicodes,
		std::vector<wxString>& utf8s);

	static int TransToLines(
		const std::vector<int>& unicodes, 
		const std::vector<wxString>& utf8s,
		const LabelStyle& style,
		std::vector<Line>& lines);

	static void DrawLines(
		const Vector& pos,
		float scale,
		const LabelStyle& style,
		const std::vector<Line>& lines,
		int tot_line_height,
		LabelLayout::Layout& layout);

	static void Draw(
		const LabelLayout::Layout* layout
		);

}; // LabelNew

}

#endif // _DRAG2D_LABEL_NEW_H_