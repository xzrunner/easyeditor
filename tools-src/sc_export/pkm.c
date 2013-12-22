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
_load_compressed_rgb(const char* filename, unsigned int* _width, unsigned int* _height) {
	char _filename[255];
	strcpy(_filename, filename);
	strcat(_filename, ".pkm");

	FILE *file = fopen(_filename, "rb");
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
    printf("load rgb %s ,size %d\n", _filename, size);
    if (data == NULL)
    {
        printf("Out of memory at %s:%i\n", __FILE__, __LINE__);
        exit(1);
    }
    fread(data, size, 1, file);

    fclose(file);  
    
    return data;  
}

static unsigned char*
_load_uncompressed_alpha(const char* filename, unsigned int* _width, unsigned int* _height) {
	char _filename[255];
	strcpy(_filename, filename);
	strcat(_filename, "_alpha.pgm");

	FILE *file = fopen(_filename, "rb");

    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int range = 0;
    int readCount = fscanf(file, "P5 %d %d %d", &width, &height, &range);
    *_width = width;
    *_height = height;
    if(readCount != 3)
    {
        printf("Error reading file header of %s", filename);
        exit(1);
    }

    if(range != 255) 
    {
        printf("Alpha file %s has wrong maximum gray value, must be 255", filename);
        exit(1);
    }

    fgetc(file);

    unsigned char *data = (unsigned char *)calloc(width * height, sizeof(unsigned char));
    size_t result = fread(data, sizeof(unsigned char), width * height, file);
    if (result != width * height) 
    {
        printf("Error reading %s", filename);
        exit(1);
    }  

    printf("load alpha %s, size: %d \n", _filename, width * height);

    return data;  
}

static int
lread(lua_State* L) {
  const char* filename = luaL_checkstring(L,1);

  unsigned int w0, h0;
  unsigned char* alpha_data = _load_uncompressed_alpha(filename, &w0, &h0);

  unsigned int w1, h1;
  unsigned char* rgb_data = _load_compressed_rgb(filename, &w1, &h1);

  assert(w0 == w1 && h0 == h1);

  lua_pushinteger(L, w0);
  lua_pushinteger(L, h0);
  lua_pushlstring(L, (void*)rgb_data, (w0*h0)>>1);
  lua_pushlstring(L, (void*)alpha_data, w0*h0);

  free(rgb_data);
  free(alpha_data);

  return 4;
}

int
luaopen_pkm(lua_State* L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{ "read", lread },
		{ NULL, NULL },
	};
	luaL_newlib(L,l);
	return 1;
}