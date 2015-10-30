#include "tl_freetype.h"
#include "tl_typedef.h"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftimage.h>

struct font {
	FT_Library library;
	FT_Face face;
};

#define MAX_FONTS 8

struct freetype {
	struct font	fonts[8];
	int count;
};

struct glyph_hash_key {
	int unicode;
	
};

static struct freetype FT;

void 
tl_ft_init() {
	FT.count = 0;
}

int 
tl_ft_add_font(const char* filepath) {
	if (FT.count >= MAX_FONTS) {
		return -1;
	}	

	struct font* f = &FT.fonts[FT.count++];
	FT_Init_FreeType(&f->library);
	if (FT_New_Face(f->library, filepath, 0, &f->face)) {
		return FT.count - 1;
	} else {
		return -1;
	}
}

uint8_t* 
tl_ft_gen_char(int unicode, int font, int size, bool edge, struct tl_glyph_layout* layout) {
	if (font < 0 || font >= FT.count) {
		return NULL;
	}

	FT_Face ft_face = FT.fonts[font].face;
	
	FT_Set_Pixel_Sizes(ft_face, size, size);
	FT_Size_Metrics s = ft_face->size->metrics;
	layout->metrics_height = s.height >> 6;

	FT_UInt gindex = FT_Get_Char_Index(ft_face, unicode);
	if(gindex == 0) {
		return NULL;
	}
	if (FT_Load_Glyph(ft_face, gindex, FT_LOAD_DEFAULT)) {
		return NULL;
	}

	FT_Glyph glyph;
	if (FT_Get_Glyph(ft_face->glyph, &glyph)) {
		return NULL;
	}

	FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
	FT_Bitmap* bitmap = &bitmap_glyph->bitmap; //just to make things easier

	// Get metrics
	layout->bearing_x = ft_face->glyph->metrics.horiBearingX >> 6;
	layout->bearing_y = ft_face->glyph->metrics.horiBearingY >> 6;
	layout->sizer.height = bitmap->rows;
	layout->sizer.width = bitmap->width;
	layout->advance = ft_face->glyph->metrics.horiAdvance >> 6;

// 	int sz = sizeof(uint32_t) * bitmap->rows * bitmap->width;
// 	uint32_t* buf = malloc(sz);
// 	if (!buf) {
// 		FT_Done_Glyph(glyph);
// 		return NULL;
// 	}
// 	memset(buf, 0, sz);
// 
// 	uint8_t col_r = (col >> 24) & 0xff;
// 	uint8_t col_g = (col >> 16) & 0xff;
// 	uint8_t col_b = (col >> 8) & 0xff;
// 
// 	int ptr = 0;
// 	for (int i = 0; i < bitmap->rows; ++i) {
// 		for (int j = 0; j < bitmap->width; ++j) {
// 			unsigned char 
// 				r = ((col_r * bitmap->buffer[ptr]) >> 8) + 1,
// 				g = ((col_g * bitmap->buffer[ptr]) >> 8) + 1,
// 				b = ((col_b * bitmap->buffer[ptr]) >> 8) + 1,
// 				a = bitmap->buffer[ptr];
// 			int dst_ptr = (bitmap->rows - 1 - i) * bitmap->width + j;
// 			buf[dst_ptr] = a << 24 | b << 16 | g << 8 | r;
// 			++ptr;
// 		}
// 	}

	int sz = sizeof(uint8_t) * bitmap->rows * bitmap->width;
	uint8_t* buf = malloc(sz);
	if (!buf) {
		FT_Done_Glyph(glyph);
		return NULL;
	}
	memset(buf, 0, sz);

	int ptr = 0;
	for (int i = 0; i < bitmap->rows; ++i) {
		for (int j = 0; j < bitmap->width; ++j) {
			unsigned char a = bitmap->buffer[ptr];
			int dst_ptr = (bitmap->rows - 1 - i) * bitmap->width + j;
			buf[dst_ptr] = a;
			++ptr;
		}
	}

	FT_Done_Glyph(glyph);

	return buf;
}