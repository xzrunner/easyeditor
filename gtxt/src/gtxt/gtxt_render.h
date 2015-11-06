#ifdef __cplusplus
extern "C"
{
#endif

#ifndef gametext_rendrer_h
#define gametext_rendrer_h

#include <stdbool.h>
#include <stdint.h>

struct gtxt_render_style {
	uint32_t color;
	int size;
	int font;
	bool edge;
};

void gtxt_render_init(struct dtex_cg* cg);

void gtxt_draw_glyph(int unicode, struct gtxt_render_style* style, float x, float y);

#endif // gametext_rendrer_h

#ifdef __cplusplus
}
#endif