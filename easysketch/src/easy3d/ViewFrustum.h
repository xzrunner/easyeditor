#ifndef _E3D_VIEW_FRUSTUM_H_
#define _E3D_VIEW_FRUSTUM_H_

#include "Vector.h"

namespace e3d
{

class ViewFrustum
{
public:
	static ivec2 TransPos3ProjectToScreen(const vec3& proj, float width, float height);

}; // ViewFrustum

}

#endif // _E3D_VIEW_FRUSTUM_H_