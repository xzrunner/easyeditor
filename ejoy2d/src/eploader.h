#ifdef __cplusplus
extern "C"
{
#endif

#ifndef EJOY_PIC_H
#define EJOY_PIC_H

#include <stdint.h>

struct picture_part {
  int32_t texid;
//  int16_t src[8];
  float src[8];
  int32_t screen[8];
};

struct picture {
  int32_t n;
  struct picture_part part[0];
};

struct label {
  uint32_t color;
  const char * font;
  int size;
  int align;
  int width;
  int height;
};

struct animation_part {
  int32_t id;
  const char * name;
  struct label * text;
};

struct animation_component {
  int16_t index;
  int16_t flag;
  uint32_t color;
  uint32_t additive;
  int32_t *mat;
};

struct animation_frame {
  int n;
  struct animation_component *c;
};

struct animation_action {
  const char * name;
  int n;
  struct animation_frame *frame;
};

struct animation {
  int part_n;
  struct animation_part * part;
  int32_t * clipbox;
  int action_n;
  struct animation_action action[0];
};

/*
  How to draw a frame :
  x = while animation->action[x].name == action name
  index(0..n) = animation->action[x].frame[n]->c[0..n].index
  id = animation->part[index].id
  draw picture (id)
*/

struct alloc;

struct export {
  const char * name;
  int id;
};

struct ejoypic {
  struct alloc * alloc;
  int export_n;
  struct export * export;
  int max_id;
  struct animation ** spr;
};

struct ejoypic * ejoypic_import(void *data, int sz, int cap);
void ejoypic_free(struct ejoypic * ep);

size_t ejoypic_capacity(void* data, int sz);

#endif

#ifdef __cplusplus
}
#endif