#include "ViewFrustum.h"
#include "Camera.h"

namespace e3d
{

ivec2 ViewFrustum::TransPos3ProjectToScreen(const vec3& proj, float screen_width, float screen_height)
{
	ivec2 pos;
	pos.x = (int)(Camera::CAM_NEAR * proj.x / proj.z);
	pos.y = (int)(Camera::CAM_NEAR * proj.y / proj.z);
	return pos;
}

vec3 ViewFrustum::TransPos3ScreenToProject(const ivec2& scr, float proj_z, float screen_width, float screen_height)
{
	vec3 pos;
	pos.x = (scr.x * proj_z) / Camera::CAM_NEAR;
	pos.z = (scr.y * proj_z) / Camera::CAM_NEAR;
	return pos;
}

vec3 ViewFrustum::TransPos3ScreenToDir(const ivec2& screen, float screen_width, float screen_height)
{
	vec3 start(0, 0, 0);

	vec3 end;
 	end.z = -Camera::CAM_NEAR;
	end.x = (screen.x - screen_width * 0.5f) * 2.0f / screen_width;
	end.y = -(screen.y - screen_height * 0.5f) * 2.0f / screen_width;

	return end - start;
}

}