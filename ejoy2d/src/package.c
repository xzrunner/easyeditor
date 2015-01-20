#include "package.h"
#include "shader.h"
#include "fault.h"

#include "LzmaDec.h"
//#include "LzAlloc.h"

#include "opengl.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "platform.h"

#define MAX_PACKAGE 128
#define MAX_TEXTURE 8

#define LZMA_PROPS_SIZE 5

static void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
static void SzFree(void *p, void *address) { p = p; MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };

static int
LzmaUncompress(unsigned char *dest, size_t  *destLen, const unsigned char *src, size_t  *srcLen,
  const unsigned char *props, size_t propsSize)
{
  ELzmaStatus status;
  return LzmaDecode(dest, destLen, src, srcLen, props, (unsigned)propsSize, LZMA_FINISH_ANY, &status, &g_Alloc);
}

struct block {
  uint8_t size[4];
  uint8_t prop[LZMA_PROPS_SIZE];
  uint8_t data[119];
};

static struct ej_package * P[MAX_PACKAGE];

#define IS_POT(x) (((x) & ((x) -1)) == 0)

//   texture
typedef enum {
  Texture2DPixelFormat_RGBA8888,
  Texture2DPixelFormat_RGBA4444,
  Texture2DPixelFormat_PVRTC4,
  Texture2DPixelFormat_PVRTC2,
  Texture2DPixelFormat_RGB888,
  Texture2DPixelFormat_RGB565,


  Texture2DPixelFormat_Default = Texture2DPixelFormat_RGBA8888,
} Texture2DPixelFormat;

static GLuint
texture_create(const void* data,Texture2DPixelFormat pixel_format,int pixel_width,int pixel_height)
{
  printf("texture_create: (%d, %d)\n", pixel_width, pixel_height);

  if ((pixel_format == Texture2DPixelFormat_RGBA8888) ||
    ( IS_POT(pixel_width) && IS_POT(pixel_height))) {
    glPixelStorei(GL_UNPACK_ALIGNMENT,4);
  } else {
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  }
  GLuint tex = 0;
  glGenTextures(1, &tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

  switch(pixel_format) {
    case Texture2DPixelFormat_RGBA8888:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)pixel_width, (GLsizei)pixel_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      break;
    case Texture2DPixelFormat_RGB888:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)pixel_width, (GLsizei)pixel_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      break;
    case Texture2DPixelFormat_RGBA4444:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)pixel_width, (GLsizei)pixel_height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);
      break;
    case Texture2DPixelFormat_RGB565:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)pixel_width, (GLsizei)pixel_height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
      break;
    default:
      return 0;
  }

  return tex;
}

static GLuint
pvrt_texture_create(uint8_t* data, size_t sz, int internal_format, int pixel_width,int pixel_height){
  printf("pvrt_texture_create: (%d, %d)\n", pixel_width, pixel_height);
  GLuint tex = 0;
  glGenTextures(1, &tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  
  uint8_t* head = data;
  int i;
  for(i=0; head-data<sz; i++){
    int size = head[0] | head[1]<<8 | head[2]<<16 | head[3]<<24;
    uint8_t* p = head+4;
    glCompressedTexImage2D(GL_TEXTURE_2D, i, internal_format, pixel_width, pixel_height, 0, size, p);
		
		GLenum err = glGetError();
		if (err != GL_NO_ERROR){
			printf("Error uploading compressed texture level: %d. glError: 0x%04X", i, err);
			return 0;
		}
    head += 4+size;
  }

  assert(head-data==sz);
  return tex;
}

static GLuint
ktx_texture_create(uint8_t* data, size_t sz, int pixel_width, int pixel_height, int pixel_size) {
#ifdef __ANDROID__
  printf("ktx_texture_create: (%d, %d)\n", pixel_width, pixel_height);
  GLuint tex = 0;
  glGenTextures(1, &tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

  glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, pixel_width, pixel_height, 0, pixel_size, data);

  return tex;
#else
  return 0;
#endif
}

static void
pkm_texture_create(uint8_t* data, size_t sz, int width, int height, GLuint* id_rgb, GLuint* id_alpha) {
#ifdef __ANDROID__
  printf("pkm_texture_create: (%d, %d)\n", width, height);

  // load rgb
  glActiveTexture(GL_TEXTURE0);
  *id_rgb = 0;
  glGenTextures(1, id_rgb);
  glBindTexture(GL_TEXTURE_2D, *id_rgb);

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  
  unsigned int size_rgb = (width * height) >> 1;
  glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, width, height, 0, size_rgb, data);

  // load alpha
  glActiveTexture(GL_TEXTURE1);
  *id_alpha = 0;
  glGenTextures(1, id_alpha);
  glBindTexture(GL_TEXTURE_2D, *id_alpha);

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0,GL_LUMINANCE,GL_UNSIGNED_BYTE, data+size_rgb);
  glGenerateMipmap(GL_TEXTURE_2D);

#endif
}

static void
pkmc_texture_create(uint8_t* data, size_t sz, int width, int height, GLuint* id_rgb, GLuint* id_alpha) {
#ifdef __ANDROID__
  printf("pkmc_texture_create: (%d, %d)\n", width, height);

  // load rgb
  glActiveTexture(GL_TEXTURE0);
  *id_rgb = 0;
  glGenTextures(1, id_rgb);
  glBindTexture(GL_TEXTURE_2D, *id_rgb);

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  
  unsigned int size_rgb = (width * height) >> 1;
  glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, width, height, 0, size_rgb, data);

  // load alpha
  glActiveTexture(GL_TEXTURE1);
  *id_alpha = 0;
  glGenTextures(1, id_alpha);
  glBindTexture(GL_TEXTURE_2D, *id_alpha);

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

  unsigned int size_alpha = (width * height) >> 1;
  glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, width, height, 0, size_alpha, data+size_rgb);

#endif
}

#ifdef __APPLE__
static void
load_pvrtc(uint8_t* buffer, size_t sz, struct texture* tex){
  int internal_format = buffer[0];
  int width = buffer[1] | buffer[2]<<8;
  int height = buffer[3] | buffer[4]<<8;
  
  switch (internal_format) {
    case 4:
      internal_format = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
      break;
    case 2:
      internal_format = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
      break;
    default:
      assert(0);
      break;
  }
  
  tex->id = pvrt_texture_create(buffer+5, sz-5, internal_format, width, height);
  tex->id_alpha = 0;
  tex->width = 1.0f/width;
  tex->height = 1.0f/height;
  tex->format = PVRTC;
}
#endif

static void
load_ktx(uint8_t* buffer, size_t sz, struct texture* tex) {
  int width = buffer[0] | buffer[1]<<8;
  int height = buffer[2] | buffer[3]<<8;
  int size = buffer[4] | buffer[5]<<8 | buffer[6]<<16 | buffer[7]<<24;
  tex->id = ktx_texture_create(buffer+8, sz-8, width, height, size);
  tex->id_alpha = 0;
  tex->width = 1.0f/width;
  tex->height = 1.0f/height;
  tex->format = KTX;
}

static void
load_pkm(uint8_t* buffer, size_t sz, struct texture* tex) {
#ifdef __ANDROID__  
  int width = buffer[0] | buffer[1]<<8;
  int height = buffer[2] | buffer[3]<<8;
  GLuint id_rgb, id_alpha;
  pkm_texture_create(buffer+4, sz-4, width, height, &id_rgb, &id_alpha);
  tex->id = id_rgb;
  tex->id_alpha = id_alpha;
  tex->width = 1.0f/width;
  tex->height = 1.0f/height;
  tex->format = PKM;
#endif
}

static void
load_pkmc(uint8_t* buffer, size_t sz, struct texture* tex) {
#ifdef __ANDROID__  
  int width = buffer[0] | buffer[1]<<8;
  int height = buffer[2] | buffer[3]<<8;
  GLuint id_rgb, id_alpha;
  pkmc_texture_create(buffer+4, sz-4, width, height, &id_rgb, &id_alpha);
  tex->id = id_rgb;
  tex->id_alpha = id_alpha;
  tex->width = 1.0f/width;
  tex->height = 1.0f/height;
  tex->format = PKM;
#endif
}

static void
load_tex4(uint8_t *buffer, size_t sz, struct texture *tex) {
  int width = buffer[0] | buffer[1]<<8;
  int height = buffer[2] | buffer[3]<<8;
  if (sz-4 != width*height*2) {
    fault("Invalid size (%d * %d * 2) %d\n",width, height, (int)(sz-4));
  }
  tex->id = texture_create(buffer+4, Texture2DPixelFormat_RGBA4444, width, height);
  tex->id_alpha = 0;
  tex->width = 1.0f/width;
  tex->height = 1.0f/height;
  tex->format = TEXTURE4;
}

static void
load_tex8(uint8_t *buffer, size_t sz, struct texture *tex) {
  int width = buffer[0] | buffer[1]<<8;
  int height = buffer[2] | buffer[3]<<8;
  if (sz-4 != width*height*4) {
    fault("Invalid size (%d * %d * 4) %d\n",width,height,(int)(sz-4));
  }
  tex->id = texture_create(buffer+4, Texture2DPixelFormat_RGBA8888, width, height);
  tex->id_alpha = 0;
  tex->width = 1.0f/width;
  tex->height = 1.0f/height;
  tex->format = TEXTURE8;
}

static int
compstr(const void *a, const void *b) {
  const struct export *aa = a;
  const struct export *bb = b;
  return strcmp(aa->name,bb->name);
}

static void
sort_ep(struct ejoypic *ep) {
  qsort(ep->export, ep->export_n, sizeof(struct export), compstr);
}

static struct ejoypic *
load_pack(uint8_t *buffer, size_t sz) {
  uint32_t cap = buffer[0] | buffer[1]<<8 | buffer[2]<<16 | buffer[3]<<24;
  struct ejoypic * ep = ejoypic_import(buffer+4, sz - 4, cap);
  if (ep == NULL) {
    fault("Error in detail");
    return NULL; // for static analyze
  }
  sort_ep(ep);
  return ep;
}

static void
loadbuffer(uint8_t * buffer, size_t sz, struct ej_package *pack) {
  int type = buffer[0];
  switch(type) {
  case TEXTURE4:
    load_tex4(buffer+1, sz-1, &pack->tex[pack->texture_n++]);
    break;
  case TEXTURE8:
    load_tex8(buffer+1, sz-1, &pack->tex[pack->texture_n++]);
    break;
  case PVRTC:
#ifdef __APPLE__
    load_pvrtc(buffer+1, sz-1, &pack->tex[pack->texture_n++]);
#endif
    break;
  case KTX:
    load_ktx(buffer+1, sz-1, &pack->tex[pack->texture_n++]);
    break;
  case PKM:
    load_pkm(buffer+1, sz-1, &pack->tex[pack->texture_n++]);
    break;
  case PKMC:
    load_pkmc(buffer+1, sz-1, &pack->tex[pack->texture_n++]);
    break;    
  case DETAIL:
    if(pack->ep)
      ejoypic_free(pack->ep);
    pack->ep = load_pack(buffer+1, sz-1);
    break;
  default:
    fault("Invalid section type %d\n",type);
  }
}

// n 表示文件中包括的记录数量
static size_t
checkmaxsize(struct FileHandle *h, int *n) {
  int32_t sz = 0;
  size_t maxsize = 0;
  *n = 0;
  for (;;) {
    if (pf_fileread(h, &sz, sizeof(sz)) == 0) {
      return maxsize;
    }
    ++*n;

    // 不加密
    if (sz<0) {
      sz = -sz;
      if (sz > maxsize) {
        maxsize = sz;
      }
      pf_fileseek_from_cur(h, sz);
    }

    // 加密
    else {
      uint8_t size[4];
      if (pf_fileread(h, size, sizeof(size)) == 0) {
        *n = 0;
        return 0;
      }
      size_t text_size = size[0] << 24 | size[1] << 16 | size[2] << 8 | size[3];
      text_size += sz + 7;
      if (text_size > maxsize) {
        maxsize = text_size;
      }
      pf_fileseek_from_cur(h, (int)(sz - 4));
    }
  }
}

static struct ej_package *
_new_ej_package(int tex){
  struct ej_package* pack = malloc(sizeof(*pack)+tex*sizeof(struct texture));
  pack->name = NULL;
  pack->texture_n = 0;
  pack->ep = NULL;
  return pack;
}

static struct ej_package *
decode(struct FileHandle *h, struct ej_package *pack, char* tmp, int n) {
  assert(pack);
  assert(tmp);

  int i;
  for (i=0;i<n;i++) {
    int32_t sz = 0;
    pf_fileread(h, &sz, sizeof(sz));
    if (sz<0) {
      unsigned char *buffer = (unsigned char *)tmp;
      if (pf_fileread(h, buffer, -sz) != 1) {
        fault("Invalid uncompress data source\n");
      }
      loadbuffer(buffer, -sz, pack);
    } else {
      struct block * block = (struct block *)tmp;
      if (pf_fileread(h, block, sz) != 1) {
        fault("Invalid compress data source\n");
      }
      if (sz <= 4 + LZMA_PROPS_SIZE) {
        fault("Invalid compress data source\n");
      }
      size_t text_size = block->size[0] << 24 | block->size[1] << 16 | block->size[2] << 8 | block->size[3];
      unsigned char * buffer = (unsigned char *)(tmp + ((sz + 3) & ~3));
      size_t src_sz = sz - sizeof(block->size) - LZMA_PROPS_SIZE;
      int r = LzmaUncompress(buffer, &text_size, block->data, &src_sz, block->prop, LZMA_PROPS_SIZE);
      if (r != SZ_OK) {
        fault("Uncompress error %d\n",r);
      }
      loadbuffer(buffer, text_size, pack);
    }
  }

  return pack;
}

static struct ej_package *
load(const char *name, const char * path[], int count, struct ej_package *pack) {
  printf("load package (%s)\n", name);
  int i;
  struct {
    struct FileHandle* fp;
    int n;
  }file_info[count];

  if(count <= 0) {
    return NULL;
  }
  
  size_t tmpsz = 0, n=0;
  char* tmp = NULL;
  memset(file_info, 0, sizeof(file_info));
  
  // check sum
  for(i=0; i<count; i++){
    file_info[i].fp = pf_fileopen(path[i], "rb");
    if (file_info[i].fp == NULL)
      fault("Can't open name: %s file: %s\n", name, path[i]);
    size_t sz;
    int bn;
    sz = checkmaxsize(file_info[i].fp, &bn);
    if (sz == 0)
      fault("block size is 0.\n");
    file_info[i].n = bn;
    if (tmpsz < sz)
      tmpsz = sz;
    n += bn;
  }
  
  if (pack == NULL){
    pack = _new_ej_package(n-1);
    if(pack == NULL)
      fault("Invalid package %s", name);
  }else{
    assert(pack->texture_n == 0);
  }
  
  // create block buffer
  tmp = malloc(tmpsz);
  
  // decode
  for(i=0; i<count; i++){
    pf_fileseek_from_head(file_info[i].fp, 0);
    decode(file_info[i].fp, pack, tmp, file_info[i].n);
    printf("\t\tfile:%s\n", path[i]);
    pf_fileclose(file_info[i].fp);
  }
  
  // set name
  if (pack->name == NULL)
    pack->name = _strdup(name);
  
  // free block buffer
  free(tmp);
  
  return pack;
}

static void
unload_texture(struct ej_package *p) {
  int i;
  for (i=0;i<p->texture_n;i++) {
    GLuint id = p->tex[i].id;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);     
    glDeleteTextures(1,&id);
    p->tex[i].id = 0;
  }
  p->texture_n = 0;
}

////////////////////////////////////////////////////////////////////////////////
//
//  PUBLIC
//
////////////////////////////////////////////////////////////////////////////////

struct ej_package *
package_load(const char * name, const char * path[], int count) {
  int i;
  int free_slot = -1;
  for (i=0;i<MAX_PACKAGE;i++) {
    struct ej_package * p = P[i];
    if (p == NULL) {
      // 找到第一个空的槽位
      if (free_slot == -1) {
        free_slot = i;
      }
    }
    else {
      if (strcmp(name, p->name)==0) {
        return p;
      }
    }
  }

  // 没有空的槽位了
  if (free_slot == -1) {
    fault("Too many package : %s", name);
    return NULL;
  }

  if(count <= 0 || path==NULL)
    return NULL;
  else{
    struct ej_package * p = load(name, path, count, NULL);
    if(p) {
      P[free_slot] = p;
    }
    return p;
  }
}

void
package_unload_all() {
  int i;
  for (i=0;i<MAX_PACKAGE;i++) {
    struct ej_package * p = P[i];
    if (p != NULL) {
      unload_texture(p);
      if(p->name){
        free(p->name);
        p->name = NULL;
      }
      free(p);
      P[i] = NULL;
    }
  }
}

void
package_unload(const char * name) {
  int i;
  for (i=0;i<MAX_PACKAGE;i++) {
    struct ej_package * p = P[i];
    if (p != NULL && strcmp(name, p->name)==0) {
      unload_texture(p);
      if(p->name){
        free(p->name);
        p->name = NULL;
      }
      free(p);
      P[i] = NULL;
      return;
    }
  }
}

