#ifndef _DRAG2D_LABEL_H_
#define _DRAG2D_LABEL_H_

#include "DFont.h"
#include "FTRender.h"

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

	void DrawGlyph(int unicode, int x, int y, int size,unsigned int color, int is_edge);

private:
	class Layout
	{
	public:
		

	}; // Layout

private:
	static const int TEX_WIDTH = 1024;
	static const int TEX_HEIGHT = 2048;

private:
	GLuint m_tex;

	float m_scale;

	DFont m_dfont;

	FTRender m_freetype;

}; // Label

}

#endif // _DRAG2D_LABEL_H_