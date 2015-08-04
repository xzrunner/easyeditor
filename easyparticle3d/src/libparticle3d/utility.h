#ifndef _EASYPARTICLE3D_UTILITY_H_
#define _EASYPARTICLE3D_UTILITY_H_

#include <drag2d.h>
#include <easy3d.h>

namespace eparticle3d
{

d2d::Vector TransCoords3To2(const float position[3]);
d2d::Vector TransCoords3To2(const float position[3], const mat4& direction);

}

#endif // _EASYPARTICLE3D_UTILITY_H_