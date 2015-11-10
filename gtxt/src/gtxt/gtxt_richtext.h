#ifdef __cplusplus
extern "C"
{
#endif

#ifndef gametext_richtext_h
#define gametext_richtext_h

#include <stdint.h>
#include <stdbool.h>

struct gtxt_richtext_style {
	uint32_t color;
	int size;
	int font;
	bool edge;

	void* ext_sym_ud;
};

void gtxt_richtext_add_font(const char* name);

void gtxt_richtext_ext_sym_cb_init(void* (*create)(const char* str),
								   void (*release)(void* ud),
								   void (*size)(void* ud, int* width, int* height), 
								   void (*render)(void* ud, float x, float y));
void gtxt_ext_sym_get_size(void* ud, int* width, int* height);
void gtxt_ext_sym_render(void* ud, float x, float y);

void gtxt_richtext_parser(const char* str, struct gtxt_label_style* style, 
						  int (*cb)(const char* str, struct gtxt_richtext_style* style, void* ud), void* ud);

#endif // gametext_richtext_h

#ifdef __cplusplus
}
#endif