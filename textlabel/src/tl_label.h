#ifdef __cplusplus
extern "C"
{
#endif

#ifndef textlabel_label_h
#define textlabel_label_h

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

	int width;
	int height;

	color_rgba color;

	int align_h;
	int align_v;
	
	int spacing_h;
	int spacing_v;
};

void tl_label_draw(const char* str, struct tl_label_style* style);

#endif // textlabel_label_h

#ifdef __cplusplus
}
#endif