#include "ViewFrustum.h"

namespace libsketch
{

// mat4 ViewFrustum::GetProjectMatrix(int width, int height) const
// {
// 	float hh = 1.0f * height / width;
// 	return mat4::Frustum(-1, 1, -hh, hh, NEAR, FAR);
// }

ivec2 ViewFrustum::TransPos3ProjectToScreen(const vec3& proj, float width, float height)
{
	ivec2 pos;

	float _near = 1000;
	float _far = 1010;

	pos.x = _near * proj.x / _far;
	pos.y = _near * proj.y / _far;

	return pos;
}

}