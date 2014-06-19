#ifndef _DRAG2D_LABEL_H_
#define _DRAG2D_LABEL_H_

#include "DFont.h"
#include "FTRender.h"
#include "WXRender.h"

#include <opengl/opengl.h>

namespace d2d
{

class Label
{
public:
	Label();
	~Label();

	void Flush();

	void Draw(const char *str, int width, int height, int align, int x, int y, int font_size,
		unsigned long color, unsigned int tcolor, unsigned int tadditive, int is_edge);

private:
	void Init();

	void LoadFontFile(const char* filename);

	void DrawGlyph(int unicode, int x, int y, int font_size, unsigned int color, int is_edge) const;

private:
	struct Layout
	{
	public:
		Layout(DFont& dfont);

		void LoadFontFile(const char* filename);

		GlyphSizer GetGlyphSizer(int unicode, const char* utf8, int font_size, int color, int is_edge);

		void InitLayout(const char* str, int width, int height, int font_size, unsigned long color, int is_edge);

	private:
		GlyphSizer GenChar(int unicode, const char* utf8, int font_size, int color, int is_edge);
		uint32_t* GenFTChar(int unicode, const char* utf8, int font_size, int color, int is_edge, GlyphLayout& layout);
		uint32_t* GenWXChar(int unicode, const char* utf8, int font_size, int color, int is_edge, GlyphLayout& layout);

	public:
//		int char_count;
		int line_height;
		int line_count;

		static const int CHAR_MAX_COUNT = 2048;
		struct Graphy
		{
			int unicode;
			float width;
		};
		Graphy char_buf[CHAR_MAX_COUNT];
		
		static const int MAX_LINE = 128;
		int line_buf[MAX_LINE];

	private:
		static const int FONT_SIZE_COUNT = 64;

	private:
		DFont& m_dfont;

		FTRender m_ft_render;
		WXRender m_wx_render;

		GlyphSizer m_space_sizer[FONT_SIZE_COUNT];

	}; // Layout

private:
	static const int TEX_WIDTH = 1024;
	static const int TEX_HEIGHT = 2048;

private:
	GLuint m_tex;

	float m_scale;

	// todo: move to layout
	DFont m_dfont;

	Layout m_layout;

}; // Label

}

#endif // _DRAG2D_LABEL_H_