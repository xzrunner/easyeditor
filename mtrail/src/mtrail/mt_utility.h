#ifdef __cplusplus
extern "C"
{
#endif

#ifndef motion_trail_utility_h
#define motion_trail_utility_h

#include <stdint.h>

#define PTR_SIZE_DIFF (8 - sizeof(void *))
#define SIZEOF_POINTER 8

struct mt_vec2 {
	union {
		struct {
			float x, y;
		};

		float xy[2];
	};
};

struct mt_color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

#endif // motion_trail_utility_h

#ifdef __cplusplus
}
#endif