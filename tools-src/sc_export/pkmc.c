#include "lua.h"
#include "lauxlib.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

typedef struct PKM_header_t {
	unsigned char identifier[8];
    unsigned char paddedWidthMSB;
    unsigned char paddedWidthLSB;
    unsigned char paddedHeightMSB;
    unsigned char paddedHeightLSB;
    unsigned char widthMSB;
    unsigned char widthLSB;
    unsigned char heightMSB;
    unsigned char heightLSB;
} PKM_header;

static unsigned char*
_load_compressed(const char* filename, unsigned int* _width, unsigned int* _height) {
  FILE *file = fopen(filename, "rb");
    if(file == NULL)
    {
        printf("Failed to open '%s'\n", filename);
        exit(1);
    }

    PKM_header header;
    fread(&header, sizeof(PKM_header), 1, file);

    unsigned int width = (header.paddedWidthMSB << 8) | header.paddedWidthLSB,
                 height = (header.paddedHeightMSB << 8) | header.paddedHeightLSB;
    *_width = width;
    *_height = height;
    unsigned int size = (width*height) >> 1;
    unsigned char* data = (unsigned char *)malloc(size);
    printf("load %s ,size %d\n", filename, size);
    if (data == NULL)
    {
        printf("Out of memory at %s:%i\n", __FILE__, __LINE__);
        exit(1);
    }
    fread(data, size, 1, file);

    fclose(file);  
    
    return data;  
}

static int
lread(lua_State* L) {
  const char* filename = luaL_checkstring(L,1);

  char _filename[255];
  strcpy(_filename, filename);
  strcat(_filename, ".pkm");
  unsigned int w0, h0;
  unsigned char* rgb_data = _load_compressed(_filename, &w0, &h0);

  strcpy(_filename, filename);
  strcat(_filename, "_alpha.pkm");
  unsigned int w1, h1;
  unsigned char* alpha_data = _load_compressed(_filename, &w1, &h1);

  assert(w0 == w1 && h0 == h1);

  lua_pushinteger(L, w0);
  lua_pushinteger(L, h0);
  lua_pushlstring(L, (void*)rgb_data, (w0*h0)>>1);
  lua_pushlstring(L, (void*)alpha_data, (w0*h0)>>1);

  free(rgb_data);
  free(alpha_data);

  return 4;
}

int
luaopen_pkmc(lua_State* L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{ "read", lread },
		{ NULL, NULL },
	};
	luaL_newlib(L,l);
	return 1;
}