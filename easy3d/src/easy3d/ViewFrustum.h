#ifndef _EASY3D_VIEW_FRUSTUM_H_
#define _EASY3D_VIEW_FRUSTUM_H_

namespace e3d
{

class ViewFrustum
{
public:
	static sm::ivec2 TransPos3ProjectToScreen(const sm::vec3& proj, float screen_width, float screen_height);
	static sm::vec3 TransPos3ScreenToProject(const sm::ivec2& scr, float proj_z, float screen_width, float screen_height);

	static sm::vec3 TransPos3ScreenToDir(const sm::ivec2& screen, float screen_width, float screen_height);

}; // ViewFrustum

}

#endif // _EASY3D_VIEW_FRUSTUM_H_