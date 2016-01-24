#ifdef __cplusplus
extern "C"
{
#endif

#ifndef shader_lab_sprite_h
#define shader_lab_sprite_h

#include <stdint.h>

void sl_sprite_load();
void sl_sprite_unload();

void sl_sprite_bind();
void sl_sprite_unbind();

void sl_sprite_projection(int width, int height);
void sl_sprite_modelview(float x, float y, float sx, float sy);

void sl_sprite_set_color(uint32_t color, uint32_t additive);
void sl_sprite_set_map_color(uint32_t rmap, uint32_t gmap, uint32_t bmap);

void sl_sprite_draw(const float* positions, const float* texcoords, int texid);

void sl_sprite_commit();

#endif // shader_lab_sprite_h

#ifdef __cplusplus
}
#endif