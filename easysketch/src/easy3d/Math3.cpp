#include "Math3.h"

namespace e3d
{

#define NUMDIM	3
#define RIGHT	0
#define LEFT	1
#define MIDDLE	2

bool Math3::HitBoundingBox(const vec3& min_box, const vec3& max_box, 
						   const vec3& origin, const vec3& dir, vec3* coord)
{
	vec3 cross;

	char quadrant[3];
	float candidate_plane[3];
	bool inside = true;

	/* Find candidate planes; this loop can be avoided if
   	rays cast all from the eye(assume perpsective view) */
	for (int i = 0; i < 3; ++i)
	{
		if (origin[i] < min_box[i]) {
			quadrant[i] = LEFT;
			candidate_plane[i] = min_box[i];
			inside = false;
		} else if (origin[i] > max_box[i]) {
			quadrant[i] = RIGHT;
			candidate_plane[i] = max_box[i];
			inside = false;
		} else {
			quadrant[i] = MIDDLE;
		}
	}

	/* Ray origin inside bounding box */
	if (inside) {
		cross = origin;
		return true;
	}

	float max_t[3];
	/* Calculate T distances to candidate planes */
	for (int i = 0; i < 3; ++i) {
		if (quadrant[i] != MIDDLE && dir[i] != 0) {
			max_t[i] = (candidate_plane[i]-origin[i])/dir[i];
		} else {
			max_t[i] = -1;
		}
	}

	/* Get largest of the maxT's for final choice of intersection */
	int which_plane = 0;
	for (int i = 1; i < 3; ++i) {
		if (max_t[which_plane] < max_t[i]) {
			which_plane = i;
		}
	}

	/* Check final candidate actually inside box */
	if (max_t[which_plane] < 0) {
		return false;
	}
	for (int i = 0; i < 3; ++i) {
		if (which_plane != i) {
			cross[i] = origin[i] + max_t[which_plane] * dir[i];
			if (cross[i] < min_box[i] || cross[i] > max_box[i]) {
				return false;
			}
		} else {
			cross[i] = candidate_plane[i];
		}
	}

	if (coord) {
		*coord = cross;
	}

	return false;
}

}