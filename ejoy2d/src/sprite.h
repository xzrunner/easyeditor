#ifndef ejoy_sprite_h
#define ejoy_sprite_h

#include <stdint.h>

#define EJ_INVISIBLE 0x1
#define EJ_MIRROR 0x2
#define EJ_MESSAGE 0x4
#define EJ_MATRIX 0x8
#define EJ_ADDITIVEMODE 0x10
#define EJ_GRAY 0x20
#define EJ_EXT_CLICK_AREA 0x40

struct picture;

struct screen_coord {
  float wscale;
  float hscale;
  int xoffset;
  int yoffset;
  float inv_scale;
};

struct ej_cursor {
  int x;
  int y;
};

struct ej_rect {
  int xmin, ymin;
  int xmax, ymax;
};

struct ej_sprite {
  struct ej_package * pack;
  struct animation * ani;
  int frame;
  int extra;
  short action;
  short flag;
  int mat[6];
  uint32_t color_trans;
  uint32_t color_additive;
  struct ej_sprite * c[0];
};

static inline void
matrix_mul(int m[6], const int m1[6], const int m2[6]) {
  m[0] = (m1[0] * m2[0] + m1[1] * m2[2]) /1024;
  m[1] = (m1[0] * m2[1] + m1[1] * m2[3]) /1024;
  m[2] = (m1[2] * m2[0] + m1[3] * m2[2]) /1024;
  m[3] = (m1[2] * m2[1] + m1[3] * m2[3]) /1024;
  m[4] = (m1[4] * m2[0] + m1[5] * m2[2]) /1024 + m2[4];
  m[5] = (m1[4] * m2[1] + m1[5] * m2[3]) /1024 + m2[5];
}

static inline int
_inverse_scale(int m[6],int o[6]) {
  if(m[0] == 0 || m[3] == 0) {
    return 1;
  }
  o[0] = (1024 * 1024) / m[0];
  o[1] = 0;
  o[2] = 0;
  o[3] = (1024 * 1024) / m[3];
  o[4] = - (m[4] * o[0]) / 1024;
  o[5] = - (m[5] * o[3]) / 1024;
  return 0;
}

static inline int
_inverse_rot(int m[6], int o[6]) {
  if(m[1] == 0 || m[2] == 0) {
    return 1;
  }
  o[0] = 0;
  o[1] = (1024 * 1024) / m[2];
  o[2] = (1024 * 1024) / m[1];
  o[3] = 0;
  o[4] = - (m[5] * o[2]) / 1024;
  o[5] = - (m[4] * o[1]) / 1024;
  return 0;
}

static inline int
matrix_inverse(int m[6], int o[6]) {
  if (m[1] == 0 && m[2] == 0) {
    return _inverse_scale(m,o);
  }
  if (m[0] == 0 && m[3] == 0) {
    return _inverse_rot(m,o);
  }
  int t = m[0] * m[3] - m[1] * m[2] ;
  if (t == 0) {
    return 1;
  }
  o[0] = (int64_t)m[3] * (1024 * 1024) / t;
  o[1] = - (int64_t)m[1] * (1024 * 1024) / t;
  o[2] = - (int64_t)m[2] * (1024 * 1024) / t;
  o[3] = (int64_t)m[0] * (1024 * 1024) / t;
  o[4] = - (m[4] * o[0] + m[5] * o[2]) / 1024;
  o[5] = - (m[4] * o[1] + m[5] * o[3]) / 1024;
  return 0;
}

struct screen_coord * sprite_default_coord(struct screen_coord * screen);
struct screen_coord * sprite_coord(struct screen_coord * screen, int x, int y, float scale); 
void screen_clip_push(int *mat, int x, int y, int *clipbox, float scale, int test);
void screen_clip_pop(int test);
int screen_clip_test(int x, int y);
int screen_clip_test_rect(int x1, int y1, int x2, int y2);
int sprite_id(struct ej_package *pack, const char *name);
int sprite_size(struct ej_package *pack, int id);
int sprite_create(struct ej_sprite * spr, struct ej_package *pack, int id);
void sprite_draw(struct ej_package * pack, struct picture * pic, int x, int y, int *mat, unsigned int color, unsigned int additive, int mirror, int shader,struct screen_coord *screen);
int sprite_find(struct ej_sprite *spr, struct ej_sprite *node, char *buf, int cap);
int sprite_test(struct ej_package * pack, struct picture * pic, int x, int y, int *mat, struct ej_cursor *c, int offx, int offy, float scale, int mirror, int extend);
void sprite_region(struct ej_package* pack, struct picture* pic, int x, int y, int *mat, int offx, int offy, float scale, int mirror, struct ej_rect* rect);

#endif
