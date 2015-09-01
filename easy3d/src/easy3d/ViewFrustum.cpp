#include "ViewFrustum.h"
#include "Camera.h"

namespace e3d
{

ivec2 ViewFrustum::TransPos3ProjectToScreen(const vec3& proj, float screen_width, float screen_height)
{
	float x = Camera::CAM_NEAR * proj.x / proj.z * screen_width * 0.5f ,
		  y = Camera::CAM_NEAR * proj.y / proj.z * screen_height * 0.5f;
	ivec2 pos;
	pos.x = floor(screen_width * 0.5f - x + 0.5f);
	pos.y = floor(screen_height * 0.5f + y + 0.5f);
	return pos;
}

vec3 ViewFrustum::TransPos3ScreenToProject(const ivec2& scr, float proj_z, float screen_width, float screen_height)
{
	vec3 pos;
	pos.x = -((scr.x / screen_width * 2 - 1) * proj_z) / Camera::CAM_NEAR;
	pos.y = ((scr.y / screen_height * 2 - 1) * proj_z) / Camera::CAM_NEAR;
	pos.z = 0;
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