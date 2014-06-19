#include "Label.h"

#include "render/Shader.h"

namespace d2d
{

Label::Label()
	: m_tex(0)
	, m_scale(1)
	, m_dfont(TEX_WIDTH, TEX_HEIGHT)
	, m_layout(m_dfont)
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
	m_layout.LoadFontFile(filename);
}

void Label::DrawGlyph(int unicode, int x, int y, int size,unsigned int color, int is_edge)
{
	
}

//////////////////////////////////////////////////////////////////////////
// class Label::Layout
//////////////////////////////////////////////////////////////////////////

Label::Layout::
Layout(DFont& dfont)
	: m_dfont(dfont)
{
}

void Label::Layout::
LoadFontFile(const char* filename)
{
	m_ft_render.LoadFont(filename);
}

GlyphSizer Label::Layout::
GetGlyphSizer(int unicode, const char* utf8, int font_size, int color, int is_edge)
{
	GlyphSizer sizer;
	if (unicode == '\n' || unicode=='\r') {
		return sizer;
	}

	if (unicode == ' ') {
		sizer = m_space_sizer[font_size];
		if (sizer.width == 0 || sizer.height == 0) {
			sizer = m_space_sizer[font_size] = m_ft_render.GetGlyphSizer(unicode, font_size);
		}
		if (sizer.width == 0 || sizer.height == 0) {
			sizer = m_space_sizer[font_size] = m_wx_render.GetGlyphSizer(unicode, font_size);
		}
		return sizer;
	}

	const struct DFont::dfont_rect* rect = m_dfont.LookUp(unicode, font_size, color, is_edge);
	if (rect) {
		sizer.width  = rect->w - 1;
		sizer.height = rect->metrics_height + 1;
	} else {
		sizer = GenChar(unicode, utf8, font_size, color, is_edge);
	}
	return sizer;
}

static inline int
copystr(char *utf8, const char *str, int n) 
{
	int i;
	utf8[0] = str[0];
	int unicode = utf8[0] & ((1 << (8-n)) - 1);
	for (i=1;i<n;i++) {
		utf8[i] = str[i];
		unicode = unicode << 6 | (utf8[i] & 0x3f);
	}
	utf8[i] = 0;
	return unicode;
}

void Label::Layout::
InitLayout(const char* str, int width, int height, int font_size, unsigned long color, int is_edge)
{
	int unicode;
	char utf8[7];
	int char_w = 0;  // 字符宽
	int char_c = 0;  // 字符数
	int line_h = 0;  // 行高

	for (int i=0; str[i]; ) 
	{
		uint8_t c = (uint8_t)str[i];
		if ((c&0x80) == 0) {
			unicode = copystr(utf8, str+i, 1);
			i += 1;
		} else if ((c&0xe0) == 0xc0) {
			unicode = copystr(utf8, str+i, 2);
			i += 2;
		} else if ((c&0xf0) == 0xe0) {
			unicode = copystr(utf8, str+i, 3);
			i += 3;
		} else if ((c&0xf8) == 0xf0) {
			unicode = copystr(utf8, str+i, 4);
			i += 4;
		} else if ((c&0xfc) == 0xf8) {
			unicode = copystr(utf8, str+i, 5);
			i += 5;
		} else {
			unicode = copystr(utf8, str+i, 6);
			i += 6;
		}

		if (unicode == '\r')
			continue;

// 		char_w = draw_size(unicode, utf8, size, color, is_edge, &is_init);
// 		if (line_h == 0) {
// 			line_h = _get_font_size(unicode, utf8, size, color, is_edge, &is_init).height;
// 		}
// 
// 		glyph_buf[char_c].data = unicode;
// 		glyph_buf[char_c].w = char_w;
// 		char_c += 1;
	}	
}

GlyphSizer Label::Layout::
GenChar(int unicode, const char* utf8, int font_size, int color, int is_edge)
{
	if(unicode == '\n') {
		return GlyphSizer();
	}

	GlyphLayout layout;
	uint32_t* buffer = NULL;
	buffer = GenFTChar(unicode, utf8, font_size, color, is_edge, layout);
	if(!buffer) {
		buffer = GenWXChar(unicode, utf8, font_size, color, is_edge, layout);
	}

	if (!buffer || layout.sizer.width <= 0 || layout.sizer.height <= 0) {
		return GlyphSizer();
	}

	DFont::dfont_rect * rect = m_dfont.Insert(unicode, font_size, color, is_edge, 
		layout.sizer.width+1, layout.metrics_height+1);
	if (rect == NULL) {
		return GlyphSizer();
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	m_dfont.CopyBuffer(buffer, rect->x, rect->y, layout.sizer.width, layout.sizer.height);
	free(buffer);

	return GlyphSizer(layout.sizer.width - 1, layout.metrics_height + 1);
}

uint32_t* Label::Layout::
GenFTChar(int unicode, const char* utf8, int font_size, int color, int is_edge, GlyphLayout& layout)
{
	uint32_t* buffer = NULL;
	if (is_edge) 
	{
		union Pixel32 cfont, coutline;
		cfont.Set(color & 0xff, (color >> 8) & 0xff, (color >> 16) & 0xff, color >> 24);
		coutline.Set(0, 0, 0, 255);
		buffer = m_ft_render.WriteGlyphWithStroker(unicode, font_size, cfont, coutline, 2, layout);
	} 
	else 
	{
		union Pixel32 col;
		col.Set(color & 0xff, (color >> 8) & 0xff, (color >> 16) & 0xff, color >> 24);
		buffer = m_ft_render.WriteGlyphNoStroker(unicode, font_size, col, layout);
	}
	return buffer;
}

uint32_t* Label::Layout::
GenWXChar(int unicode, const char* utf8, int font_size, int color, int is_edge, GlyphLayout& layout)
{
	return NULL;
}

}