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
};

void gtxt_richtext_parser(const char* str, struct gtxt_label_style* style, 
						  int (*cb)(const char* str, struct gtxt_richtext_style* style, void* ud), void* ud);

#endif // gametext_richtext_h

#ifdef __cplusplus
}
#endif