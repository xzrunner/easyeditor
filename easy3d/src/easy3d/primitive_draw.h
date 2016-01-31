#ifndef _E3D_PRIMITIVE_DRAW_H_
#define _E3D_PRIMITIVE_DRAW_H_



#include "Vector.h"
#include "Matrix.h"

namespace e3d
{

class AABB;

void DrawLine(const vec3& p0, const vec3& p1, ee::Colorf color = ee::BLACK);

void DrawTriLine(const vec3& p0, const vec3& p1, const vec3& p2, ee::Colorf color = ee::BLACK);

void DrawCube(const AABB& aabb, ee::Colorf color = ee::BLACK);
void DrawCube(const mat4& mat, const AABB& aabb, ee::Colorf color = ee::BLACK);
void DrawCube(const vec3& min, const vec3& max, ee::Colorf color = ee::BLACK);
void DrawCube(const vec3& min, const vec3& max, int texid);

void DrawCross(const vec3& center, const vec3& size, ee::Colorf color = ee::BLACK);

void DrawGrids(const vec3& min, const vec3& max, const vec3& size, ee::Colorf color = ee::BLACK);

void DrawPoints(const std::vector<vec3>& points, ee::Colorf color = ee::BLACK);

}

#endif // _E3D_PRIMITIVE_DRAW_H_