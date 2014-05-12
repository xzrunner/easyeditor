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
		Font();
		virtual ~Font();

		bool loadFromFile(const wxString& filepath);

//		void setSize(unsigned int h);

	public:
		static const int DEFAULT_SIZE = 16;

	private:
		void make_dlist(char ch);

	public:
		unsigned int h;			///< Holds the height of the font.
		unsigned int * textures;	///< Holds the texture id's 
		unsigned int list_base;	///< Holds the first display list id

		FT_Face face;

	private:
		wxString m_filename;

	}; // Font
}

