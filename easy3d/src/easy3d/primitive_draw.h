#ifndef _EASY3D_PRIMITIVE_DRAW_H_
#define _EASY3D_PRIMITIVE_DRAW_H_

#include <ee/Vector3D.h>
#include <ee/Color.h>
#include <ee/style_config.h>
#include <ee/Matrix3D.h>

#include <vector>

namespace e3d
{

class AABB;

void DrawLine(const ee::vec3& p0, const ee::vec3& p1, ee::Colorf color = ee::BLACK);

void DrawTriLine(const ee::vec3& p0, const ee::vec3& p1, const ee::vec3& p2, ee::Colorf color = ee::BLACK);

void DrawCube(const AABB& aabb, ee::Colorf color = ee::BLACK);
void DrawCube(const ee::mat4& mat, const AABB& aabb, ee::Colorf color = ee::BLACK);
void DrawCube(const ee::vec3& min, const ee::vec3& max, ee::Colorf color = ee::BLACK);
void DrawCube(const ee::vec3& min, const ee::vec3& max, int texid);

void DrawCross(const ee::vec3& center, const ee::vec3& size, ee::Colorf color = ee::BLACK);

void DrawGrids(const ee::vec3& min, const ee::vec3& max, const ee::vec3& size, ee::Colorf color = ee::BLACK);

void DrawPoints(const std::vector<ee::vec3>& points, ee::Colorf color = ee::BLACK);

}

#endif // _EASY3D_PRIMITIVE_DRAW_H_