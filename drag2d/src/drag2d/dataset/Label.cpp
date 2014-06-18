#include "Label.h"

#include "render/Shader.h"

namespace d2d
{

Label::Label()
	: m_tex(0)
	, m_scale(1)
	, m_dfont(TEX_WIDTH, TEX_HEIGHT)
{
	Init();
}

Label::~Label()
{
	glDeleteTextures(1, &m_tex);
}

void Label::Flush()
{
	if (m_dfont.IsDirty()) 
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)TEX_WIDTH, (GLsizei)TEX_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_dfont.GetBuffer());
	}
	m_dfont.Flush();
}

void Label::Draw(const char *str, int width, int height, int align, int x, int y, int font_size,
				 unsigned long color, unsigned int tcolor, unsigned int tadditive, int is_edge)
{
	Shader* shader = Shader::Instance();
	shader->sprite();
//	shader->color();
	
	//int i, j;
	//int cx = 0, cy = 0;

	//int halign = align & 0x0f;
	//int valign = align >> 4;

	//switch (valign) {
	//case 0:
	//	cy = 0;
	//	break;
	//case 1:
	//	cy = height - layout_line_h * layout_line_c;
	//	break;
	//default:
	//	cy = (height - layout_line_h * layout_line_c) / 2;
	//	break;
	//}
	//cy = MAX(cy, 0);

	//j = TEXT_RTL?layout_char_c-1:0;
	//for (i=0; i<layout_line_c; ++i) {
	//	if (halign == 3)
	//		halign = TEXT_RTL;
	//	switch (halign) {
	//  case 0:
	//	  cx = 0;
	//	  break;
	//  case 1:
	//	  cx = width - line_buf[i].w;
	//	  break;
	//  default:
	//	  cx = (width - line_buf[i].w) / 2;
	//	  break;
	//	}

	//	if (TEXT_RTL) {
	//		for (; j>line_buf[i].end_idx; --j) {
	//			int unicode = glyph_buf[j].data;
	//			if (unicode != '\n' && unicode != '\r') {
	//				draw_utf8(unicode, x, y, cx + line_buf[i].w - glyph_buf[j].w, cy, mat, size, color, screen, is_edge, use_cache);
	//				cx -= glyph_buf[j].w;
	//			}
	//		}
	//	} else {
	//		for (; j<line_buf[i].end_idx; ++j) {
	//			int unicode = glyph_buf[j].data;
	//			if (unicode != '\n' && unicode != '\r') {
	//				draw_utf8(unicode, x, y, cx, cy, mat, size, color, screen, is_edge, use_cache);
	//				cx += glyph_buf[j].w;
	//			}
	//		}
	//	}

	//	cy += layout_line_h;
	//	if (cy > height)  // 下一行已经不能显示
	//		break;
	//}
}

void Label::Init()
{
	m_tex = 0;
	m_scale = 1;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glGenTextures(1, &m_tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_tex);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)TEX_WIDTH, (GLsizei)TEX_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

void Label::LoadFontFile(const char* filename)
{
	m_dfont.Clear();

	m_freetype.LoadFont(filename);
}

void Label::DrawGlyph(int unicode, int x, int y, int size,unsigned int color, int is_edge)
{

}

}