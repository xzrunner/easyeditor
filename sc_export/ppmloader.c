#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

static void
gen8(FILE *color, FILE *alpha, uint8_t *buffer, int size) {
	int i;
	for (i=0;i<size;i++) {
		int red = fgetc(color);
		int green = fgetc(color);
		int blue = fgetc(color);
		int a = fgetc(alpha);
		buffer[i*4] = (uint8_t)red;
		buffer[i*4+1] = (uint8_t)green;
		buffer[i*4+2] = (uint8_t)blue;
		buffer[i*4+3] = (uint8_t)a;
	}
}

static void
gen4(FILE *color, FILE *alpha, uint16_t *buffer, int size) {
	int i;
	for (i=0;i<size;i++) {
		int red = fgetc(color);
		int green = fgetc(color);
		int blue = fgetc(color);
		int a = fgetc(alpha);
		buffer[i] = (uint16_t) (a | blue << 4 | green << 8 | red << 12);
	}
}

static int
lppm(lua_State *L) {
	size_t sz = 0;
	const char * filename = luaL_checklstring(L, 1, &sz);
	char tmp[sz + 8];
	sprintf(tmp,"%s.ppm",filename);
	FILE *color = fopen(tmp,"rb");
	if (color == NULL) {
		return luaL_error(L, "Can't open %s\n", tmp);
	}
	sprintf(tmp,"%s.alpha.pgm",filename);
	FILE *alpha = fopen(tmp,"rb");
	if (alpha == NULL) {
		fclose(color);
		return luaL_error(L, "Can't open %s\n", tmp);
	}

	int w=0,h=0,depth=0;
	int w2=0,h2=0,depth2=0;
	fscanf(color, "P6\n%d %d\n%d\n", &w, &h, &depth);
	fscanf(alpha, "P5\n%d %d\n%d\n", &w2, &h2, &depth2);
	if (w!=w2 || h!=h2 || depth!=depth2) {
		fclose(color);
		fclose(alpha);
		return luaL_error(L, "Image %s mismatch", tmp);
	}
	lua_pushinteger(L,w);
	lua_pushinteger(L,h);
	lua_pushinteger(L,depth);
	void *buffer = NULL;
	switch(depth) {
	case 15:
		buffer = malloc(w*h*2);
		gen4(color,alpha,buffer,w*h);
		lua_pushlstring(L, buffer, w*h*2);
		break;
	case 255:
		buffer = malloc(w*h*4);
		gen8(color,alpha,buffer,w*h);
		lua_pushlstring(L, buffer, w*h*4);
		break;
	default:
		fclose(color);
		fclose(alpha);
		return luaL_error(L, "Image %s unsupport depth %d", tmp, depth);
	}

	fclose(color);
	fclose(alpha);
	return 4;
}

int
luaopen_ppm(lua_State *L) {
	luaL_checkversion(L);
	lua_pushcfunction(L, lppm);

	return 1;
}
