#ifdef __cplusplus
extern "C"
{
#endif

#ifndef textlabel_freetype_h
#define textlabel_freetype_h

#include <stdbool.h>
#include <stdint.h>

struct tl_glyph_layout;

void tl_ft_init();

int tl_ft_add_font(const char* filepath);

uint8_t* tl_ft_gen_char(int unicode, int font, int size, bool edge, struct tl_glyph_layout*);

#endif // textlabel_freetype_h

#ifdef __cplusplus
}
#endif