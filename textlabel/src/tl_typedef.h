#ifdef __cplusplus
extern "C"
{
#endif

#ifndef textlabel_typedef_h
#define textlabel_typedef_h

#include <stdint.h>

typedef uint32_t color_rgba;

struct tl_glyph_sizer {
	float width;
	float height;
};

struct tl_glyph_layout {
	struct tl_glyph_sizer sizer;
	float bearing_x, bearing_y;
	float advance;
	float metrics_height;
};

enum tl_hori_align {
	HA_LEFT		= 0,
	HA_RIGHT	= 1,
	HA_CENTER	= 2,
	HA_AUTO		= 3
};

enum tl_vert_align {
	VA_TOP		= 0,
	VA_BOTTOM	= 1,
	VA_CENTER	= 2,
	VA_AUTO		= 3
};

struct tl_label_style {
	int font_size;
	int width, height;
	color_rgba color;
	int align_h;
	int align_v;
};

#endif // textlabel_typedef_h

#ifdef __cplusplus
}
#endif