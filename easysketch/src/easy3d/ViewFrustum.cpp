#include "ViewFrustum.h"
#include "Camera.h"

namespace e3d
{

ivec2 ViewFrustum::TransPos3ProjectToScreen(const vec3& proj, float width, float height)
{
	ivec2 pos;
	pos.x = (int)(Camera::CAM_NEAR * proj.x / Camera::CAM_FAR);
	pos.y = (int)(Camera::CAM_NEAR * proj.y / Camera::CAM_FAR);
	return pos;
}

vec3 ViewFrustum::TransPos3ScreenToDir(const ivec2& screen, float width, float height)
{
	vec3 start(0, 0, 0);

	vec3 end;
 	end.z = -Camera::CAM_NEAR;
	end.x = (screen.x - width * 0.5f) * 2.0f / width;
	end.y = -(screen.y - height * 0.5f) * 2.0f / width;

	return end - start;
}

}