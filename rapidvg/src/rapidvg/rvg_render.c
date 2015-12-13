#include "rvg_render.h"
#include "rvg_shader.h"

void 
rvg_rect(float xmin, float ymin, float xmax, float ymax) {
	rvg_shader_type(TYPE_LINE_LOOP);

 	float coords[8];
 	coords[0] = xmin; coords[1] = ymin;
 	coords[2] = xmax; coords[3] = ymin;
 	coords[4] = xmax; coords[5] = ymax;
 	coords[6] = xmin; coords[7] = ymax;
 	rvg_shader_add_vertices(coords, 4);
}