#ifndef _DRAG2D_LABEL_LAYOUT_H_
#define _DRAG2D_LABEL_LAYOUT_H_

#include "common/Vector.h"

#include <map>
#include <vector>

namespace d2d
{

class LabelLayout
{
public:
	static LabelLayout* Instance();

public:
	struct Glyph
	{
		Vector vertices[4], texcoords[4];
	}; // Glyph

	struct Layout
	{
		const char* text;
		std::vector<Glyph> glyphs;
	}; // Layout

public:
	const Layout* Query(const char* text) const;
	void Insert(const char* text, const Layout* layout);

private:
	LabelLayout();
	~LabelLayout();

private:
	std::map<const char*, const Layout*> m_str2layout;

private:
	static LabelLayout* m_instance;

}; // LabelLayout

}

#endif // _DRAG2D_LABEL_LAYOUT_H_