#ifdef __cplusplus
extern "C"
{
#endif

#ifndef rigging_animation_h
#define rigging_animation_h

#include "rg_timeline.h"

#include <stdint.h>

struct rg_skeleton;

struct rg_animation {
	struct rg_skeleton* sk;

	struct rg_timeline timeline;

	int max_frame;
};

#define SIZEOF_RG_ANIM (sizeof(struct rg_animation) + PTR_SIZE_DIFF * 2)

#endif // rigging_animation_h

#ifdef __cplusplus
}
#endif