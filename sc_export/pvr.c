
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"

#define MAX(A,B)  ((A) > (B) ? (A) : (B))
#define PVR_TEXTURE_FLAG_TYPE_MASK  0xff

#define COMPRESSED_RGBA_PVRTC_4BPPV1_IMG 4
#define COMPRESSED_RGBA_PVRTC_2BPPV1_IMG 2
static char prv_texture_identifier[4] = "PVR!";

enum{
  kPVRTextureFlagTypePVRTC_2 = 24,
  kPVRTextureFlagTypePVRTC_4
};


struct PVRTexHeader{
  uint32_t headerLength;
  uint32_t height;
  uint32_t width;
  uint32_t numMipmaps;
  uint32_t flags;
  uint32_t dataLength;
  uint32_t bpp;
  uint32_t bitmaskRed;
  uint32_t bitmaskGreen;
  uint32_t bitmaskBlue;
  uint32_t bitmaskAlpha;
  uint32_t pvrTag;
  uint32_t numSurfs;
};

struct slice{
  int size;
  uint8_t data[2048*2048];
};

struct pvr_texture{
  struct slice image_data[10];
  int image_data_count;
  
  int internal_format;
  int width;
  int height;
  int has_alpha;
} PT;


static int pvrt_unpack(uint8_t* data);

static inline void
_image_data_clear_all(){
  PT.image_data_count = 0;
}

static inline void
_image_data_add(uint8_t* p, int size){
  assert(PT.image_data_count <10);
  assert(size <sizeof(PT.image_data[0].data));
  
  PT.image_data[PT.image_data_count].size = size;
  uint8_t* _p = PT.image_data[PT.image_data_count].data;
  
  memcpy(_p, p, size);
  PT.image_data_count++;
}

static inline int
_image_data_count(){
  return PT.image_data_count;
}

static inline struct slice*
_image_data_idx(int idx){
  assert(idx>=0 && idx<10);
  return &(PT.image_data[idx]);
}

static inline void _pvrt_init(){
  PT.width = PT.height = 0;
  PT.internal_format = COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
  PT.has_alpha = 0;
  _image_data_clear_all();
}

static inline uint32_t
_read_l32tohost(uint8_t* p){
  assert(p);
  uint32_t ret = 0;
  ret = ret | p[0];
  ret = ret | (((int)p[1])<<8);
  ret = ret | (((int)p[2])<<16);
  ret = ret | (((int)p[3])<<24);
  return ret;
}

static struct PVRTexHeader*
_read_head(uint8_t* p, struct PVRTexHeader* head){
  assert(p);
  assert(head);

  head->headerLength = _read_l32tohost(p),    p+=sizeof(uint32_t);
  head->height = _read_l32tohost(p),          p+=sizeof(uint32_t);
  head->width = _read_l32tohost(p),           p+=sizeof(uint32_t);
  head->numMipmaps = _read_l32tohost(p),      p+=sizeof(uint32_t);
  head->flags = _read_l32tohost(p),           p+=sizeof(uint32_t);
  head->dataLength = _read_l32tohost(p),      p+=sizeof(uint32_t);
  head->bpp = _read_l32tohost(p),             p+=sizeof(uint32_t);
  head->bitmaskRed = _read_l32tohost(p),      p+=sizeof(uint32_t);
  head->bitmaskGreen = _read_l32tohost(p),    p+=sizeof(uint32_t);
  head->bitmaskBlue = _read_l32tohost(p),     p+=sizeof(uint32_t);
  head->bitmaskAlpha = _read_l32tohost(p),    p+=sizeof(uint32_t);
  head->pvrTag = _read_l32tohost(p),          p+=sizeof(uint32_t);
  head->numSurfs = _read_l32tohost(p),        p+=sizeof(uint32_t);
  
  return head;
}

static FILE*
pvrt_open_file(const char* file_name, uint8_t** data_p){
  assert(file_name);
  assert(data_p);
  FILE* fp = fopen(file_name, "rb");
  if(fp == NULL) return NULL;

  fseek(fp, 0L, SEEK_END);
  size_t size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  *data_p = (uint8_t*)malloc(size);
  fread(*data_p, 1, size, fp);
  return fp;
}


static void
pvrt_close_file(FILE* fp, uint8_t** data_p){
  if(fp)
    fclose(fp);

  free(*data_p);
  *data_p = NULL;
}

static int 
lua_pvrt_load(lua_State* L){
  _pvrt_init();

  const char* file_name = lua_tostring(L, 1);
  uint8_t* data = NULL;
  FILE* fp = pvrt_open_file(file_name, &data);
  
  if(fp == NULL)
    luaL_error(L, "open file: %s is erorr.\n", file_name);
  pvrt_unpack(data);

  pvrt_close_file(fp, &data);

  lua_pushinteger(L, PT.width);
  lua_pushinteger(L, PT.height);
  lua_pushinteger(L, PT.internal_format);

  lua_newtable(L);
  int i;

  printf("\nlua_pvrt_load: w=%d h=%d f=%d c=%d\n",
   PT.width, PT.height, PT.internal_format, _image_data_count());
  for(i =0; i<_image_data_count(); i++){
    struct slice* sp = _image_data_idx(i);
    lua_pushnumber(L, i+1);
    lua_pushlstring(L, (const char*)sp->data, sp->size);
    lua_settable(L, -3);
    printf("  image data[%d]: size=%d\n", i, sp->size);
  }

  return 4;
}

static int
pvrt_unpack(uint8_t* data){
  assert(data);
  
  int success = 0;
  struct PVRTexHeader header = {0};
  uint32_t flags, pvr_tag;
  uint32_t dataLength = 0, dataOffset = 0, dataSize = 0;
  uint32_t blockSize = 0, widthBlocks = 0, heightBlocks = 0;
  uint32_t width = 0, height = 0, bpp = 4;
  uint8_t *bytes = NULL;
  uint32_t formatFlags;
  _read_head(data, &header);
  
  pvr_tag = header.pvrTag;
  if (prv_texture_identifier[0] != ((pvr_tag >>  0) & 0xff) ||
      prv_texture_identifier[1] != ((pvr_tag >>  8) & 0xff) ||
      prv_texture_identifier[2] != ((pvr_tag >> 16) & 0xff) ||
      prv_texture_identifier[3] != ((pvr_tag >> 24) & 0xff))
  {
    return 0;
  }
  
  flags = header.flags;
  formatFlags = flags & PVR_TEXTURE_FLAG_TYPE_MASK;
  
  if(formatFlags == kPVRTextureFlagTypePVRTC_4 || formatFlags == kPVRTextureFlagTypePVRTC_2){
    _image_data_clear_all();
    
    if(formatFlags == kPVRTextureFlagTypePVRTC_4)
      PT.internal_format = COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
    else if(formatFlags == kPVRTextureFlagTypePVRTC_2)
      PT.internal_format = COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
    
    PT.width = width = header.width;
    PT.height = height = header.height;
    
    if(header.bitmaskAlpha)
      PT.has_alpha = 1;
    else
      PT.has_alpha = 0;
    
    dataLength = header.dataLength;
    bytes = data + sizeof(header);
    
    // Calculate the data size for each texture level and respect the minimum number of blocks
    while (dataOffset < dataLength){
      if (formatFlags == kPVRTextureFlagTypePVRTC_4){
        blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
        widthBlocks = width / 4;
        heightBlocks = height / 4;
        bpp = 4;
      }
      else{
        blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
        widthBlocks = width / 8;
        heightBlocks = height / 4;
        bpp = 2;
      }
      
      // Clamp to minimum number of blocks
      if (widthBlocks < 2)
        widthBlocks = 2;
      if (heightBlocks < 2)
        heightBlocks = 2;
      
      dataSize = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);
      
      _image_data_add(bytes+dataOffset, dataSize);
      dataOffset += dataSize;
      
      width = MAX(width >> 1, 1);
      height = MAX(height >> 1, 1);
    }
    
    success = 1;
  }
  
  return success;
}

int
luaopen_pvr(lua_State* L){
  luaL_checkversion(L);
  luaL_Reg l[] = {
    {"load", lua_pvrt_load},
    {NULL, NULL},
  };
  luaL_newlib(L,l);
  return 1;
}



