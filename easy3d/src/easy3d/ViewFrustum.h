#ifndef _E3D_VIEW_FRUSTUM_H_
#define _E3D_VIEW_FRUSTUM_H_

#include "Vector.h"

namespace e3d
{

class ViewFrustum
{
public:
	static ivec2 TransPos3ProjectToScreen(const vec3& proj, float screen_width, float screen_height);
	static vec3 TransPos3ScreenToProject(const ivec2& scr, float proj_z, float screen_width, float screen_height);

	static vec3 TransPos3ScreenToDir(const ivec2& screen, float screen_width, float screen_height);

}; // ViewFrustum

}

#endif // _E3D_VIEW_FRUSTUM_H_