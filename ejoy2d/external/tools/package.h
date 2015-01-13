#ifndef ejoy_package_h
#define ejoy_package_h

#include "eploader.h"
#include "opengl.h"

#include <stdbool.h>

#define TEXTURE4 0
#define TEXTURE8 1
#define DETAIL 2
#define PVRTC  3
#define KTX 4
#define PKM 5
#define PKMC 6
#define RRP 11
#define PTS 12

struct texture {
  GLuint id;
  GLuint id_alpha;
  float width;
  float height;
  uint8_t format;
};

struct ej_package {
  char * name;
  int texture_n;  // need reload where texture_n == 0
  struct ejoypic *ep;
  struct texture tex[0];
};

#endif
