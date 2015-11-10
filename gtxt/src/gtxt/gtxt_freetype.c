#include "gtxt_freetype.h"
#include "gtxt_typedef.h"
#include "gtxt_glyph.h"
#include "gtxt_richtext.h"

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

static struct freetype FT;

static uint8_t* BUF;
static size_t BUF_SZ;

void 
gtxt_ft_init() {
	FT.count = 0;
}

int 
gtxt_ft_add_font(const char* name, const char* filepath) {
	if (FT.count >= MAX_FONTS) {
		return -1;
	}

	struct font* f = &FT.fonts[FT.count++];

	if (FT_Init_FreeType(&f->library)) {
		return -1;
	}
	if (FT_New_Face(f->library, filepath, 0, &f->face)) {
		return -1;
	}

	gtxt_richtext_add_font(name);

	return FT.count - 1;
}

bool
_load_glyph_to_bitmap(int unicode, int font, int size, bool edge, 
                      struct gtxt_glyph_layout* layout, void (*cb_func)(FT_Bitmap* bitmap)) {
	if (font < 0 || font >= FT.count) {
		return false;
	}

	FT_Face ft_face = FT.fonts[font].face;

	FT_Set_Pixel_Sizes(ft_face, size, size);
	FT_Size_Metrics s = ft_face->size->metrics;
	layout->metrics_height = s.height >> 6;

	FT_UInt gindex = FT_Get_Char_Index(ft_face, unicode);
	if (gindex == 0) {
		return false;
	}
	if (FT_Load_Glyph(ft_face, gindex, FT_LOAD_DEFAULT)) {
		return false;
	}

	FT_Glyph glyph;
	if (FT_Get_Glyph(ft_face->glyph, &glyph)) {
		return false;
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

	if (cb_func) {
		cb_func(bitmap);
	}

	FT_Done_Glyph(glyph);

	return true;
}

static inline void
_copy_glyph_bitmap(FT_Bitmap* bitmap) {
	int sz = sizeof(uint8_t) * bitmap->rows * bitmap->width;
	if (BUF_SZ < sz) {
		free(BUF);
		BUF = malloc(sz);
		if (!BUF) {
			return;
		}
		BUF_SZ = sz;
	}

	int ptr = 0;
	for (int i = 0; i < bitmap->rows; ++i) {
		for (int j = 0; j < bitmap->width; ++j) {
			unsigned char a = bitmap->buffer[ptr];
			int dst_ptr = (bitmap->rows - 1 - i) * bitmap->width + j;
			BUF[dst_ptr] = a;
			++ptr;
		}
	}
}

void 
gtxt_ft_get_layout(int unicode, int font, int size, bool edge, struct gtxt_glyph_layout* layout) {
	_load_glyph_to_bitmap(unicode, font, size, edge, layout, NULL);
}

uint8_t* 
gtxt_ft_gen_char(int unicode, int font, int size, bool edge, struct gtxt_glyph_layout* layout) {
	bool succ = _load_glyph_to_bitmap(unicode, font, size, edge, layout, _copy_glyph_bitmap);
	return succ ? BUF : NULL;
}