#include "LzmaEnc.h"
#include "LzmaDec.h"
#include "Alloc.h"

#include <lua.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define LZMA_PROPS_SIZE 5

static void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
static void SzFree(void *p, void *address) { p = p; MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };

static int LzmaCompress(unsigned char *dest, size_t  *destLen, const unsigned char *src, size_t  srcLen,
  unsigned char *outProps, size_t *outPropsSize) {
  CLzmaEncProps props;
  LzmaEncProps_Init(&props);
  props.level = -1;
  props.dictSize = 0;
  props.lc = -1;
  props.lp = -1;
  props.pb = -1;
  props.fb = -1;
  props.numThreads = -1;

  return LzmaEncode(dest, destLen, src, srcLen, &props, outProps, outPropsSize, 0,
      NULL, &g_Alloc, &g_Alloc);
}

static int LzmaUncompress(unsigned char *dest, size_t  *destLen, const unsigned char *src, size_t  *srcLen,
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

static int
lcompress(lua_State *L) {
	size_t sz;
	const unsigned char * src = (const unsigned char * )luaL_checklstring(L,1,&sz);
	if (sz > 0xffffff00u)
		return 0;
	struct block * block = malloc(sz + sizeof(struct block));
	size_t destLen = sz + sizeof(block->data);
	size_t propLen = LZMA_PROPS_SIZE;
	int r = LzmaCompress(block->data, &destLen, src, sz, block->prop, &propLen);
	if (r != SZ_OK) {
		free(block);
		return 0;
	}
	block->size[0] = (sz >> 24) & 0xff;
	block->size[1] = (sz >> 16) & 0xff;
	block->size[2] = (sz >> 8) & 0xff;
	block->size[3] = (sz) & 0xff;
	lua_pushlstring(L, (const char *)block, destLen + sizeof(struct block) - sizeof(block->data));
	free(block);
	return 1;
}

static int
luncompress(lua_State *L) {
	size_t sz;
	const struct block * block = (const struct block *)luaL_checklstring(L,1,&sz);
	if (sz <= 4 + LZMA_PROPS_SIZE) {
		return 0;
	}
	size_t text_size = block->size[0] << 24 | block->size[1] << 16 | block->size[2] << 8 | block->size[3];
	unsigned char * buffer = malloc(text_size);
	size_t src_sz = sz - sizeof(block->size) - LZMA_PROPS_SIZE;
	int r = LzmaUncompress(buffer, &text_size, block->data, &src_sz, block->prop, LZMA_PROPS_SIZE);
	if (r != SZ_OK) {
		free(buffer);
		return 0;
	}
	lua_pushlstring(L, (const char *)buffer, text_size);
	free(buffer);
	return 1;
}

static int
lload(lua_State *L) {
	const char * aname = luaL_checkstring(L, 1);
	FILE *f = fopen(aname, "rb");
	if (f==NULL) {
		return luaL_error(L,"Can't open %s", aname);
	}
	fseek(f, 0, SEEK_END);
	int sz = ftell(f);
	fseek(f, 0 , SEEK_SET);
	if (sz <= 4 + LZMA_PROPS_SIZE) {
		return luaL_error(L,"Invalid achive %s",aname);
	}
	struct block * block = (struct block *) malloc(sz);
	if (fread(block, sz, 1, f)!=1) {
		free(block);
		return luaL_error(L,"Can't read %s",aname);
	}

	size_t text_size = block->size[0] << 24 | block->size[1] << 16 | block->size[2] << 8 | block->size[3];
	unsigned char * buffer = malloc(text_size);
	size_t src_sz = sz - sizeof(block->size) - LZMA_PROPS_SIZE;
	int r = LzmaUncompress(buffer, &text_size, block->data, &src_sz, block->prop, LZMA_PROPS_SIZE);
	if (r != SZ_OK) {
		free(buffer);
		free(block);
		return luaL_error(L, "Invalid achive %s", aname);
	}
	free(block);
	lua_pushlstring(L, (const char *)buffer, text_size);
	free(buffer);
	lua_setfield(L, LUA_REGISTRYINDEX , "LZMALIB");
	return 0;
}

struct archive_source {
	size_t sz;
	const char * source;
};

static int
find_in_archive(lua_State *L, const char *code, const char *endptr, const char *name, struct archive_source *as) {
	while (code < endptr-4) {	
		uint8_t * block = (uint8_t*)code;
		size_t text_size = block[0] << 24 | block[1] << 16 | block[2] << 8 | block[3];
		if (code + text_size > endptr-4) {
			break;
		}
		if (strcmp(name, code+4)==0) {
			int slen = strlen(name);
			as->sz = text_size - slen - 1;
			as->source = code + 4 + slen + 1;
			return 0;
		}
		code += text_size + 4;
	}
	lua_pushfstring(L, "\n\tno file in archive : %s", name);
	return 1;
}

static int
loader(lua_State *L) {
	lua_getfield(L, LUA_REGISTRYINDEX , "LZMALIB");
	size_t sz = 0;
	const char * code = luaL_checklstring(L,-1, &sz);
	size_t name_sz = 0;
	const char * libname = luaL_checklstring(L, 1, &name_sz);
	char tmp[1+name_sz+5];
	char *name = tmp+1;
	tmp[0]='@';
	int i;
	for (i=0;i<name_sz;i++) {
		if (libname[i] == '.') {
			name[i] = '/';
		} else {
			name[i] = libname[i];
		}
	}
	strcpy(name+name_sz,".lua");
	struct archive_source as;
	if (find_in_archive(L, code, code+sz, name, &as)) {
		return 1;
	}
	int r = luaL_loadbuffer(L, as.source, as.sz, tmp);
	if (r!=LUA_OK) {
		return luaL_error(L, "error loading module %s from archive file %s :\n\t%s",
			lua_tostring(L, 1), name, lua_tostring(L, -1));
	}
	lua_pushstring(L, name);
	return 2;
}

int
luaopen_lzma(lua_State *L) {
	luaL_checkversion(L);
	lua_getglobal(L, "package");
	luaL_checktype(L,-1,LUA_TTABLE);
	lua_getfield(L,-1, "searchers");
	luaL_checktype(L,-1,LUA_TTABLE);
	int len = lua_rawlen(L, -1);
	lua_pushcfunction(L, loader);
	lua_rawseti(L, -2, len+1);
	lua_pop(L,2);

	luaL_Reg l[] = {
		{ "compress", lcompress },
		{ "uncompress", luncompress },
		{ "load" , lload },
		{ NULL , NULL },
	};
	luaL_newlib(L,l);
	return 1;
}
