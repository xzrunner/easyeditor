#ifndef _EASY3D_PRIMITIVE_DRAW_H_
#define _EASY3D_PRIMITIVE_DRAW_H_

#include <ee/color_config.h>

#include <SM_Vector.h>
#include <SM_Matrix.h>

#include <vector>

namespace e3d
{

class AABB;

void DrawLine(const sm::vec3& p0, const sm::vec3& p1, s2::Color color = ee::BLACK);

void DrawTriLine(const sm::vec3& p0, const sm::vec3& p1, const sm::vec3& p2, s2::Color color = ee::BLACK);

void DrawCube(const AABB& aabb, s2::Color color = ee::BLACK);
void DrawCube(const sm::mat4& mat, const AABB& aabb, s2::Color color = ee::BLACK);
void DrawCube(const sm::vec3& min, const sm::vec3& max, s2::Color color = ee::BLACK);
void DrawCube(const sm::vec3& min, const sm::vec3& max, int texid);

void DrawCross(const sm::vec3& center, const sm::vec3& size, s2::Color color = ee::BLACK);

void DrawGrids(const sm::vec3& min, const sm::vec3& max, const sm::vec3& size, s2::Color color = ee::BLACK);

void DrawPoints(const std::vector<sm::vec3>& points, s2::Color color = ee::BLACK);

}

#endif // _EASY3D_PRIMITIVE_DRAW_H_