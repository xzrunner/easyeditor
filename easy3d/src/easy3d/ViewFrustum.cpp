#include "ViewFrustum.h"
#include "Camera.h"

namespace e3d
{

ee::ivec2 ViewFrustum::TransPos3ProjectToScreen(const ee::vec3& proj, float screen_width, float screen_height)
{
	float x = Camera::CAM_NEAR * proj.x / proj.z * screen_width * 0.5f ,
		  y = Camera::CAM_NEAR * proj.y / proj.z * screen_height * 0.5f;
	ee::ivec2 pos;
	pos.x = static_cast<int>(floor(screen_width * 0.5f - x + 0.5f));
	pos.y = static_cast<int>(floor(screen_height * 0.5f + y + 0.5f));
	return pos;
}

ee::vec3 ViewFrustum::TransPos3ScreenToProject(const ee::ivec2& scr, float proj_z, float screen_width, float screen_height)
{
	ee::vec3 pos;
	pos.x = -((scr.x / screen_width * 2 - 1) * proj_z) / Camera::CAM_NEAR;
	pos.y = ((scr.y / screen_height * 2 - 1) * proj_z) / Camera::CAM_NEAR;
	pos.z = 0;
	return pos;
}

ee::vec3 ViewFrustum::TransPos3ScreenToDir(const ee::ivec2& screen, float screen_width, float screen_height)
{
	ee::vec3 start(0, 0, 0);

	ee::vec3 end;
 	end.z = -Camera::CAM_NEAR;
	end.x = (screen.x - screen_width * 0.5f) * 2.0f / screen_width;
	end.y = -(screen.y - screen_height * 0.5f) * 2.0f / screen_width;

	return end - start;
}

}