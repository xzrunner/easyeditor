#ifdef __cplusplus
extern "C"
{
#endif

#ifndef gametext_layout_h
#define gametext_layout_h

#include <stdbool.h>

void gtxt_layout_single(int unicode, struct gtxt_label_style* lstyle, struct gtxt_richtext_style* rstyle);
void gtxt_layout_multi(struct dtex_array* unicodes, struct gtxt_label_style* style);

void gtxt_layout(struct dtex_array* unicodes, struct gtxt_label_style* style);
void gtxt_layout_end();

void gtxt_layout_traverse(void (*cb)(int unicode, int font, int size, bool edge, float x, float y));

#endif // gametext_layout_h

#ifdef __cplusplus
}
#endif