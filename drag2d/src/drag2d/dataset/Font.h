#pragma once

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

		bool loadFromFile(const wxString& filepath);

//		void setSize(unsigned int h);

	public:
		static const int DEFAULT_SIZE = 16;

	private:
		void make_dlist_freetypes(char ch);
		void make_dlist_wx(char ch);

	public:
		unsigned int h;			///< Holds the height of the font.
		unsigned int * textures;	///< Holds the texture id's 
		unsigned int list_base;	///< Holds the first display list id

	private:
		bool m_stroke;

		wxString m_filename;

		FT_Face m_face;

	}; // Font
}

