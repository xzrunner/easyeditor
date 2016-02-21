#ifndef _EASY3D_VIEW_FRUSTUM_H_
#define _EASY3D_VIEW_FRUSTUM_H_

#include <ee/Vector3D.h>

namespace e3d
{

class ViewFrustum
{
public:
	static ee::ivec2 TransPos3ProjectToScreen(const ee::vec3& proj, float screen_width, float screen_height);
	static ee::vec3 TransPos3ScreenToProject(const ee::ivec2& scr, float proj_z, float screen_width, float screen_height);

	static ee::vec3 TransPos3ScreenToDir(const ee::ivec2& screen, float screen_width, float screen_height);

}; // ViewFrustum

}

#endif // _EASY3D_VIEW_FRUSTUM_H_