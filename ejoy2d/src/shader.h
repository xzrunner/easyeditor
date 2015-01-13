#ifndef EJOY_GAME_2D_SHADER_H
#define EJOY_GAME_2D_SHADER_H

#define PROGRAM_SPRITE 0
//#define PROGRAM_TEXT 1
#define PROGRAM_GRAY 2
#define PROGRAM_SPRITE_KTX 3
#define PROGRAM_GRAY_KTX 4
#define PROGRAM_SPRITE_PKM 5
#define PROGRAM_GRAY_PKM 6

struct dtex_rect;

void shader_load();
void shader_unload();
void shader_color(unsigned int color, unsigned int additive);
void shader_blend(int mode);
void shader_texture(int id);
void shader_fbo(int id);
int shader_get_fbo();
void shader_draw(const float vb[16], int texid);
void shader_draw_separate(const float vb[16], int id_rgb, int id_alpha);
void shader_program(int n);
void shader_scissor(int x, int y, int width, int height, int enable);
void shader_flush();

#endif
