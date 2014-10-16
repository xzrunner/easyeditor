#ifndef _E3D_PRIMITIVE_DRAW_H_
#define _E3D_PRIMITIVE_DRAW_H_

#include <drag2d.h>

#include "Vector.h"

namespace e3d
{

class Cube;

void DrawCube(const Cube& cube, d2d::Colorf color = d2d::BLACK);
void DrawCube(const vec3& min, const vec3& max, d2d::Colorf color = d2d::BLACK);

void DrawCross(const vec3& center, const vec3& size, d2d::Colorf color = d2d::BLACK);

}

#endif // _E3D_PRIMITIVE_DRAW_H_