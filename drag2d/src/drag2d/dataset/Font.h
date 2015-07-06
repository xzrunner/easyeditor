#ifndef _DRAG2D_FONT_H_
#define _DRAG2D_FONT_H_

#include "common/ResourcesMgr.h"
#include "common/Object.h"

//FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>

namespace d2d
{

class Font;
typedef ResourcesMgr<Font> FontMgr;

// from nehe's FreeType
class Font : public Object
{
public:
	Font(bool stroke = false);
	virtual ~Font();

	bool LoadFromFile(const std::string& filepath);

//		void setSize(unsigned int h);

public:
	static const int DEFAULT_SIZE = 16;

	static const char* DEFAULT_FONTFILE;

private:
	void make_dlist_freetypes(char ch);
	void make_dlist_wx(char ch);

public:
	unsigned int h;			///< Holds the height of the font.
	unsigned int * textures;	///< Holds the texture id's 
	unsigned int list_base;	///< Holds the first display list id

private:
	bool m_stroke;

	std::string m_filename;

	FT_Face m_face;

}; // Font

}

#endif // _DRAG2D_FONT_H_