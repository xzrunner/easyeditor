#include "gtxt_freetype.h"
#include "gtxt_typedef.h"
#include "gtxt_glyph.h"
#include "gtxt_richtext.h"

#include <assert.h>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftimage.h>
#include <freetype/ftstroke.h>

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

static bool
_draw_default(struct font* font, FT_UInt gindex, struct gtxt_glyph_layout* layout, 
			  void (*cb_func)(FT_Bitmap* bitmap)) {
	FT_Face ft_face = font->face;

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

struct span {
	int x, y;
	int width;
	int coverage;
};

struct spans {
	struct span items[512];
	int sz;
};

static inline void
_raster_cb(const int y, const int count, const FT_Span * const spans, void * const user) {
	struct spans* sptr = (struct spans*)user;
	for (int i = 0; i < count; ++i) {
		struct span* s = &sptr->items[sptr->sz];
		s->x = spans[i].x;
		s->y = y;
		s->width = spans[i].len;
		s->coverage = spans[i].coverage;
		++sptr->sz;
	}
}

static inline void
_draw_spans(FT_Library library, FT_Outline* outline, struct spans* spans) {
	FT_Raster_Params params;
	memset(&params, 0, sizeof(params));
	params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
	params.gray_spans = _raster_cb;
	params.user = spans;

	FT_Outline_Render(library, outline, &params);
}

struct point {
	float x, y;
};

struct rect {
	float xmin, xmax, ymin, ymax;
};

#define MAX(a, b) ( ((a)>(b))?(a):(b) )
#define MIN(a, b) ( ((a)<(b))?(a):(b) )

// static inline void
// _rect_merge_point(struct rect* r, struct point* p) {
// 	r->xmin = MIN(r->xmin, p->x);
// 	r->ymin = MIN(r->ymin, p->y);
// 	r->xmax = MAX(r->xmax, p->x);
// 	r->ymax = MAX(r->ymax, p->y);
// }

struct pixel {
	union {
		uint32_t integer;

		struct {
			uint8_t r, g, b, a;
		} rgba;
	};
};

static inline void
_rect_merge_point(struct rect* r, float x, float y) {
	r->xmin = MIN(r->xmin, x);
	r->ymin = MIN(r->ymin, y);
	r->xmax = MAX(r->xmax, x);
	r->ymax = MAX(r->ymax, y);
}

static inline float
_rect_width(struct rect* r) {
	return r->xmax - r->xmin + 1;
}

static inline float
_rect_height(struct rect* r) {
	return r->ymax - r->ymin + 1;
}

static bool
_draw_with_edge(struct font* font, FT_UInt gindex, float outline_width,
                struct gtxt_glyph_layout* layout, 
				void (*cb_func)(FT_Bitmap* bitmap)) {
	FT_Face ft_face = font->face;
	FT_Library ft_library = font->library;

	if (FT_Load_Glyph(ft_face, gindex, FT_LOAD_NO_BITMAP)) {
		return false;
	}
	
	if (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
		return false;
	}

	// Render the basic glyph to a span list.
	struct spans in_spans;
	_draw_spans(ft_library, &ft_face->glyph->outline, &in_spans);
	
	// Next we need the spans for the outline.
	struct spans out_spans;

	// Set up a stroker.
	FT_Stroker stroker;
	FT_Stroker_New(ft_library, &stroker);
	FT_Stroker_Set(stroker,
		(int)(outline_width * 64),
		FT_STROKER_LINECAP_ROUND,
		FT_STROKER_LINEJOIN_ROUND,
		0);

	FT_Glyph glyph;
	if (FT_Get_Glyph(ft_face->glyph, &glyph)) {
		return false;
	}

	FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
	// Again, this needs to be an outline to work.
	if (glyph->format == FT_GLYPH_FORMAT_OUTLINE)
	{
		// Render the outline spans to the span list
		FT_Outline *o = &((FT_OutlineGlyph)glyph)->outline;
		_draw_spans(ft_library, o, &out_spans);
	}

	// Clean up afterwards.
	FT_Stroker_Done(stroker);
	FT_Done_Glyph(glyph);

	if (in_spans.sz == 0) {
		return false;
	}

	struct rect rect;
	rect.xmin = rect.xmax = in_spans.items[0].x;
	rect.ymin = rect.ymax = in_spans.items[0].y;
	for (int i = 0; i < in_spans.sz; ++i) {
		struct span* s = &in_spans.items[i];
		_rect_merge_point(&rect, s->x, s->y);
		_rect_merge_point(&rect, s->x + s->width - 1, s->y);
	}
	for (int i = 0; i < out_spans.sz; ++i) {
		struct span* s = &out_spans.items[i];
		_rect_merge_point(&rect, s->x, s->y);
		_rect_merge_point(&rect, s->x + s->width - 1, s->y);
	}

	layout->bearing_x = ft_face->glyph->metrics.horiBearingX >> 6;
	layout->bearing_y = ft_face->glyph->metrics.horiBearingY >> 6;
	layout->advance = ft_face->glyph->metrics.horiAdvance >> 6;

	int img_w = _rect_width(&rect),
		img_h = _rect_height(&rect);
	int img_sz = img_w * img_h;

	layout->sizer.width = img_w;
	layout->sizer.height = img_h;

	struct pixel pixels[img_sz];
	memset(pixels, 0, sizeof(struct pixel) * img_sz);

// 	int sz = sizeof(struct pixel) * img_sz;
// 	struct pixel* pixels = (struct pixel*)malloc(sz);
// 	memset(pixels, 0, sz);

	// Loop over the outline spans and just draw them into the
	// image.
	for (int i = 0; i < out_spans.sz; ++i) {
		struct span* out_span = &out_spans.items[i];
		for (int w = 0; w < out_span->width; ++w) {
			int index = (int)((out_span->y - rect.ymin) * img_w + out_span->x - rect.xmin + w);
			struct pixel* p = &pixels[index];
			p->rgba.r = 255;
			p->rgba.g = 0;
			p->rgba.b = 0;
			p->rgba.a = out_span->coverage;
		}
	}

	// Then loop over the regular glyph spans and blend them into
	// the image.
	for (int j = 0; j < in_spans.sz; ++j) {
		struct span* s = &in_spans.items[j];
		for (int w = 0; w < s->width; ++w) {
			int index = (s->y - rect.ymin) * img_w + s->x - rect.xmin + w;
			struct pixel* p = &pixels[index];
		}
	}

	return true;
}

static bool
_load_glyph_to_bitmap(int unicode, int font, int size, bool edge, 
                      struct gtxt_glyph_layout* layout, void (*cb_func)(FT_Bitmap* bitmap)) {
	if (font < 0 || font >= FT.count) {
		return false;
	}

	struct font* sfont = &FT.fonts[font];
	FT_Face ft_face = sfont->face;
	assert(ft_face);

	FT_Set_Pixel_Sizes(ft_face, size, size);
	FT_Size_Metrics s = ft_face->size->metrics;
	layout->metrics_height = s.height >> 6;	

	FT_UInt gindex = FT_Get_Char_Index(ft_face, unicode);
	if (gindex == 0) {
		return false;
	}

	if (edge) {
		return _draw_with_edge(sfont, gindex, 1, layout, cb_func);
	} else {
		return _draw_default(sfont, gindex, layout, cb_func);
	}
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