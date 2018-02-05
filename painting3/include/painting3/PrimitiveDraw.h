#pragma once

#include <SM_Vector.h>
#include <SM_Matrix.h>

#include <vector>

namespace pt3
{

class AABB;

class PrimitiveDraw
{
public:
	static void Init();

	static void SetColor(uint32_t abgr);

	static void Line(const sm::vec3& p0, const sm::vec3& p1);

	static void TriLine(const sm::vec3& p0, const sm::vec3& p1, const sm::vec3& p2);

	static void Rect(const sm::vec3& p0, const sm::vec3& p1);

	static void Cube(const AABB& aabb);
	static void Cube(const sm::mat4& mat, const AABB& aabb);
	static void Cube(const sm::vec3& min, const sm::vec3& max);
	static void Cube(const sm::vec3& min, const sm::vec3& max, int texid);

	static void Cross(const sm::vec3& center, const sm::vec3& size);

	static void Grids(const sm::vec3& min, const sm::vec3& max, const sm::vec3& size);

	static void Points(const std::vector<sm::vec3>& points);

private:
	static void SetShader(int shader_type);

}; // PrimitiveDraw

}