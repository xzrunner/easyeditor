#ifdef __cplusplus
extern "C"
{
#endif

#ifndef spatial_partition_region_h
#define spatial_partition_region_h

#include "sp_typedef.h"

struct sp_region {
	float xmin, ymin;
	float xmax, ymax;
};

#define REGION_INIT(r) { \
	(r).xmin = (r).ymin = FLT_MAX; \
	(r).xmax = (r).ymax = -FLT_MAX; \
}

#define REGION_AREA(r) (((r).xmax - (r).xmin) * ((r).ymax - (r).ymin))

#define REGION_COMBINE(in1, in2, out) { \
	(out).xmin = MIN((in1).xmin, (in2).xmin); \
	(out).ymin = MIN((in1).ymin, (in2).ymin); \
	(out).xmax = MAX((in1).xmax, (in2).xmax); \
	(out).ymax = MAX((in1).ymax, (in2).ymax); \
}

#define REGION_CONTAINS(a, b) ((a).xmin <= (b).xmin && (a).xmax >= (b).xmax && (a).ymin <= (b).ymin && (a).ymax >= (b).ymax)

#define REGION_TOUCH(a, b) ((a).xmin == (b).xmin || (a).xmax == (b).xmax || (a).ymin == (b).ymin || (a).ymax == (b).ymax)

#define REGION_INTERSECT(a, b) (!((a).xmin > (b).xmax || (a).xmax < (b).xmin || (a).ymin > (b).ymax || (a).ymax < (b).ymin))

#endif // spatial_partition_region_h

#ifdef __cplusplus
}
#endif