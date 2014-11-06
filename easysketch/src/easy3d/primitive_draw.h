#ifndef _E3D_PRIMITIVE_DRAW_H_
#define _E3D_PRIMITIVE_DRAW_H_

#include <drag2d.h>

#include "Vector.h"
#include "Matrix.h"

namespace e3d
{

class AABB;

void DrawLine(const vec3& p0, const vec3& p1, d2d::Colorf color = d2d::BLACK);

void DrawTriLine(const vec3& p0, const vec3& p1, const vec3& p2, d2d::Colorf color = d2d::BLACK);

void DrawCube(const AABB& aabb, d2d::Colorf color = d2d::BLACK);
void DrawCube(const mat4& mat, const AABB& aabb, d2d::Colorf color = d2d::BLACK);
void DrawCube(const vec3& min, const vec3& max, d2d::Colorf color = d2d::BLACK);

void DrawCross(const vec3& center, const vec3& size, d2d::Colorf color = d2d::BLACK);

void DrawGrids(const vec3& min, const vec3& max, const vec3& size, d2d::Colorf color = d2d::BLACK);

}

#endif // _E3D_PRIMITIVE_DRAW_H_