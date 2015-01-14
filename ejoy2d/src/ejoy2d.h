#ifdef __cplusplus
extern "C"
{
#endif

#ifndef ejoy2d_h
#define ejoy2d_h

#define SCALE 16

struct ej_screen {
	int w;
	int h;
	float scale;
};

extern struct ej_screen screen_info;
static inline void ejoy_set_screen(struct ej_screen* info) {
	screen_info.w = info->w;
	screen_info.h = info->h;
	screen_info.scale = info->scale;
}
static inline void ejoy_get_screen(struct ej_screen* info) {
	info->w = screen_info.w;
	info->h = screen_info.h;
	info->scale = screen_info.scale;
}

struct ej_extra {
	int offx;
	int offy;
	float scale;
	void * ud;
	void * (*extra)(void *ud, int idx);
};

#endif

#ifdef __cplusplus
}
#endif