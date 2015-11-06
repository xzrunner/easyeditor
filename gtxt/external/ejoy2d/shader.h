#ifndef EJOY_2D_SHADER_H
#define EJOY_2D_SHADER_H

#define PROGRAM_DEFAULT -1
#define PROGRAM_PICTURE 0
#define PROGRAM_RENDERBUFFER 1
#define PROGRAM_TEXT 2
#define PROGRAM_TEXT_EDGE 3
#define PROGRAM_TRAIL 4

void shader_texture(int id, int channel);
void shader_draw(const struct vertex_pack vb[4],uint32_t color,uint32_t additive, 
				 uint32_t rmap, uint32_t gmap, uint32_t bmap);

#endif
