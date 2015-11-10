#ifdef __cplusplus
extern "C"
{
#endif

#ifndef gametext_freetype_h
#define gametext_freetype_h

#include <stdbool.h>
#include <stdint.h>

struct gtxt_glyph_layout;

void gtxt_ft_init();

int gtxt_ft_add_font(const char* name, const char* filepath);

void gtxt_ft_get_layout(int unicode, int font, int size, bool edge, struct gtxt_glyph_layout*);
uint8_t* gtxt_ft_gen_char(int unicode, int font, int size, bool edge, struct gtxt_glyph_layout*);

#endif // gametext_freetype_h

#ifdef __cplusplus
}
#endif