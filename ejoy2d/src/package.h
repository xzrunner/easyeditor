#ifdef __cplusplus
extern "C"
{
#endif

#ifndef ejoy_package_h
#define ejoy_package_h

#include "eploader.h"

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
#define RRR 13
#define B4R 14

typedef unsigned int GLuint;

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


// 加载一个资源包
struct ej_package * package_load(const char * name, const char * path[], int count);
// 卸载一个资源包
void package_unload(const char * name);

// 在程序完全退出, 或者执行完热更新之后调用
void package_unload_all();

#endif

#ifdef __cplusplus
}
#endif