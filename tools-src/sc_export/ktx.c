#include "lua.h"
#include "lauxlib.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#define KTX_IDENTIFIER_REF  { 0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A }
#define KTX_ENDIAN_REF      (0x04030201)
#define KTX_ENDIAN_REF_REV  (0x01020304)


typedef struct KTX_header_t {
	uint8_t  identifier[12];
	uint32_t endianness;
	uint32_t glType;
	uint32_t glTypeSize;
	uint32_t glFormat;
	uint32_t glInternalFormat;
	uint32_t glBaseInternalFormat;
	uint32_t pixelWidth;
	uint32_t pixelHeight;
	uint32_t pixelDepth;
	uint32_t numberOfArrayElements;
	uint32_t numberOfFaces;
	uint32_t numberOfMipmapLevels;
	uint32_t bytesOfKeyValueData;
} KTX_header;

#define KTX_HEADER_SIZE   (sizeof(struct KTX_header_t))

static int
lread(lua_State* L) {
  const char * filename = luaL_checkstring(L,1);

  FILE* file = fopen(filename, "rb");
  if (file) {
  	KTX_header header;
    fread(&header, KTX_HEADER_SIZE, 1, file);

    int width = header.pixelWidth;
    int height = header.pixelHeight;
    unsigned int size;
    fread(&size, sizeof(unsigned int), 1, file);

    unsigned char* data = (unsigned char*)malloc(size);
    fread(data, size, 1, file);

    lua_pushinteger(L, width);
    lua_pushinteger(L, height);
    lua_pushinteger(L, size);
    lua_pushlstring(L, (void*)data, size);
    free(data);

  	fclose(file);
  } else {
  	luaL_error(L, "read %s fail", filename);
  }

  return 4;
}

int
luaopen_ktx(lua_State* L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{ "read", lread },
		{ NULL, NULL },
	};
	luaL_newlib(L,l);
	return 1;
}