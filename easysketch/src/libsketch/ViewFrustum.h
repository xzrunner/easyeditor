#ifndef _LIBSKETCH_VIEW_FRUSTUM_H_
#define _LIBSKETCH_VIEW_FRUSTUM_H_

#include <drag2d.h>

namespace libsketch
{

class ViewFrustum
{
public:
//	static mat4 GetProjectMatrix(int width, int height);

	static ivec2 TransPos3ProjectToScreen(const vec3& proj, float width, float height);

}; // ViewFrustum

}

#endif // _LIBSKETCH_VIEW_FRUSTUM_H_