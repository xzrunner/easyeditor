#ifndef _E3D_PRIMITIVE_DRAW_H_
#define _E3D_PRIMITIVE_DRAW_H_

#include "Vector.h"

namespace e3d
{

class Cube;

void DrawCube(const Cube& cube);
void DrawCube(const vec3& min, const vec3& max);

}

#endif // _E3D_PRIMITIVE_DRAW_H_