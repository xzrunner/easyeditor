#include "lua.h"
#include "lauxlib.h"
#include "png.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

static inline int
 _round(int c){
	c = ((c+7)>0xff)?(0xff):(c+7); 
	return c >>4;
}

static void*
gen4(u_int8_t* buffer, size_t size){
	size_t i;
	assert(size%4 == 0);
	int r, g, b, a;
	uint16_t* _p = (uint16_t*)buffer;
	for(i=0; i<size; i+=4, _p++){
		r = _round(buffer[i]);  // r
		g = _round(buffer[i+1]);// g
		b = _round(buffer[i+2]);// b
		a = _round(buffer[i+3]);// a

		*_p = (uint16_t) (a | b << 4 | g << 8 | r << 12);
	}
	return buffer;
}


static int
lread(lua_State *L) {
	const char * filename = luaL_checkstring(L,1);
	const char*  modle = luaL_checkstring(L, 2);
	enum{
		GEN_8,
		GEN_4
	}_e;

	if(!strcmp(modle, "-png8")){
		_e = GEN_8;
	}else if (!strcmp(modle, "-png4")){
		_e = GEN_4;
	}else{
		luaL_error(L, "the modle is error:< -png8, -png4>.\n");
	}

	png_image image;
	memset(&image, 0, sizeof(image));
	image.version = PNG_IMAGE_VERSION;
	if (png_image_begin_read_from_file(&image, filename)) {
		image.format = PNG_FORMAT_RGBA;
		lua_pushinteger(L, image.width);
		lua_pushinteger(L, image.height);
		if(_e == GEN_8)
			lua_pushinteger(L, 255);
		else if (_e == GEN_4)
			lua_pushinteger(L, 15);

		void * buffer = malloc(PNG_IMAGE_SIZE(image));
		 if (png_image_finish_read(&image, 0, buffer, 0, NULL)) {
		 	if(_e == GEN_8)
				lua_pushlstring(L, buffer, PNG_IMAGE_SIZE(image));
			else if(_e == GEN_4)
				lua_pushlstring(L, gen4(buffer, PNG_IMAGE_SIZE(image)), PNG_IMAGE_SIZE(image)/2);
			free(buffer);
		} else {
			free(buffer);
			luaL_error(L, "read %s fail", filename);
		}
	} else {
		luaL_error(L, "read %s fail", filename);
	}

	return 4;
}

int 
luaopen_png(lua_State *L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{ "read", lread },
		{ NULL, NULL },
	};
	luaL_newlib(L,l);
	return 1;
}
