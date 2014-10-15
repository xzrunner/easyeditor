#include "ViewFrustum.h"

namespace e3d
{

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