#ifdef __cplusplus
extern "C"
{
#endif

#ifndef shader_lab_sprite_h
#define shader_lab_sprite_h

void sl_sprite_init();
void sl_sprite_bind();

void sl_sprite_projection(int width, int height);
void sl_sprite_modelview(float x, float y, float sx, float sy);

void sl_sprite_draw(float* positions, float* texcoords, int texid);

#endif // shader_lab_sprite_h

#ifdef __cplusplus
}
#endif