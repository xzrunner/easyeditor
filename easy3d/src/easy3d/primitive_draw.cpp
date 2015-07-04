#include "primitive_draw.h"

#include "ShaderMgr.h"
#include "AABB.h"

namespace e3d
{

void DrawLine(const vec3& p0, const vec3& p1, d2d::Colorf color)
{
	vec3 vertices[] = {p0, p1};

	ShaderMgr* shader = ShaderMgr::Instance();
	shader->Shape();
	shader->DrawShape(GL_LINES, &vertices[0].x, 2, color, true);
}

void DrawTriLine(const vec3& p0, const vec3& p1, const vec3& p2, d2d::Colorf color)
{
	vec3 vertices[] = {p0, p1, p2};

	ShaderMgr* shader = ShaderMgr::Instance();
	shader->Shape();
	shader->DrawShape(GL_LINE_LOOP, &vertices[0].x, 3, color, true);
}

void DrawCube(const AABB& aabb, d2d::Colorf color)
{
	DrawCube(aabb.Min(), aabb.Max(), color);
}

void DrawCube(const mat4& mat, const AABB& aabb, d2d::Colorf color)
{
	const vec3& min = aabb.Min();
	const vec3& max = aabb.Max();

	vec3 vertices[8];
	vertices[0] = mat * min; 
	vertices[1] = mat * vec3(max.x, min.y, min.z);
	vertices[2] = mat * vec3(max.x, max.y, min.z);
	vertices[3] = mat * vec3(min.x, max.y, min.z);
	vertices[4] = mat * vec3(min.x, min.y, max.z);
	vertices[5] = mat * vec3(max.x, min.y, max.z);
	vertices[6] = mat * max;
	vertices[7] = mat * vec3(min.x, max.y, max.z);

	unsigned short indices[24];
	int idx = 0;
	indices[idx++] = 0;
	indices[idx++] = 1;
	indices[idx++] = 1;
	indices[idx++] = 2;
	indices[idx++] = 2;
	indices[idx++] = 3;
	indices[idx++] = 3;
	indices[idx++] = 0;

	indices[idx++] = 4;
	indices[idx++] = 5;
	indices[idx++] = 5;
	indices[idx++] = 6;
	indices[idx++] = 6;
	indices[idx++] = 7;
	indices[idx++] = 7;
	indices[idx++] = 4;

	indices[idx++] = 0;
	indices[idx++] = 4;
	indices[idx++] = 1;
	indices[idx++] = 5;
	indices[idx++] = 2;
	indices[idx++] = 6;
	indices[idx++] = 3;
	indices[idx++] = 7;

	ShaderMgr* shader = ShaderMgr::Instance();
	shader->Shape();
	shader->DrawShape(GL_LINES, &vertices[0].x, 8, color, indices, 24);
}

void DrawCube(const vec3& min, const vec3& max, d2d::Colorf color)
{
	float vertices[24];
	int idx = 0;
	
	vertices[idx++] = min.x;
	vertices[idx++] = min.y;
	vertices[idx++] = min.z;

	vertices[idx++] = max.x;
	vertices[idx++] = min.y;
	vertices[idx++] = min.z;

	vertices[idx++] = max.x;
	vertices[idx++] = max.y;
	vertices[idx++] = min.z;

	vertices[idx++] = min.x;
	vertices[idx++] = max.y;
	vertices[idx++] = min.z;

	vertices[idx++] = min.x;
	vertices[idx++] = min.y;
	vertices[idx++] = max.z;

	vertices[idx++] = max.x;
	vertices[idx++] = min.y;
	vertices[idx++] = max.z;

	vertices[idx++] = max.x;
	vertices[idx++] = max.y;
	vertices[idx++] = max.z;

	vertices[idx++] = min.x;
	vertices[idx++] = max.y;
	vertices[idx++] = max.z;

	unsigned short indices[24];
	idx = 0;
	indices[idx++] = 0;
	indices[idx++] = 1;
	indices[idx++] = 1;
	indices[idx++] = 2;
	indices[idx++] = 2;
	indices[idx++] = 3;
	indices[idx++] = 3;
	indices[idx++] = 0;

	indices[idx++] = 4;
	indices[idx++] = 5;
	indices[idx++] = 5;
	indices[idx++] = 6;
	indices[idx++] = 6;
	indices[idx++] = 7;
	indices[idx++] = 7;
	indices[idx++] = 4;

	indices[idx++] = 0;
	indices[idx++] = 4;
	indices[idx++] = 1;
	indices[idx++] = 5;
	indices[idx++] = 2;
	indices[idx++] = 6;
	indices[idx++] = 3;
	indices[idx++] = 7;

	ShaderMgr* shader = ShaderMgr::Instance();
	shader->Shape();
	shader->DrawShape(GL_LINES, vertices, 8, color, indices, 24);
}

void DrawCube(const vec3& min, const vec3& max, int texid)
{
	float vertices[2 * 6 * 3 * 3];
	int idx_vert = 0;

	float texcoords[2 * 6 * 3 * 2];
	int idx_tex = 0;

	// front
	vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = min.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 1;
	vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = max.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	// back
	vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = min.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 1;
	vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = max.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	// left
	vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 1;
	vertices[idx_vert++] = min.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = min.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = min.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	// right
	vertices[idx_vert++] = max.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = max.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 1;
	vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	vertices[idx_vert++] = max.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	// bottom
	vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 1;
	vertices[idx_vert++] = max.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = max.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = max.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	// top
	vertices[idx_vert++] = min.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = min.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 1;
	vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	vertices[idx_vert++] = min.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = min.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 0;
	vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;	

	e3d::ShaderMgr* shader = e3d::ShaderMgr::Instance();
	shader->Sprite();

	shader->DrawTri(vertices, texcoords, 2 * 6 * 3, texid);
}

void DrawCross(const vec3& center, const vec3& size, d2d::Colorf color)
{
	float vertices[18];
	int idx = 0;

	vertices[idx++] = center.x - size.x;
	vertices[idx++] = center.y;
	vertices[idx++] = center.z;
	vertices[idx++] = center.x + size.x;
	vertices[idx++] = center.y;
	vertices[idx++] = center.z;

	vertices[idx++] = center.x;
	vertices[idx++] = center.y - size.y;
	vertices[idx++] = center.z;
	vertices[idx++] = center.x;
	vertices[idx++] = center.y + size.y;
	vertices[idx++] = center.z;

	vertices[idx++] = center.x;
	vertices[idx++] = center.y;
	vertices[idx++] = center.z - size.x;
	vertices[idx++] = center.x;
	vertices[idx++] = center.y;
	vertices[idx++] = center.z + size.z;

	ShaderMgr* shader = ShaderMgr::Instance();
	shader->Shape();
	shader->DrawShape(GL_LINES, vertices, 6, color, true);
}

void DrawGrids(const vec3& min, const vec3& max, const vec3& size, d2d::Colorf color)
{
	std::vector<vec3> lines;
	int cx = size.x ? std::ceil(max.x - min.x) / size.x : 1;
	int cy = size.y ? std::ceil(max.y - min.y) / size.y : 1;
	int cz = size.z ? std::ceil(max.z - min.z) / size.z : 1;
	lines.reserve(cx * cy * cz * 2);

	for (float z = min.z; z <= max.z; z += size.z) {
		for (float x = min.y; x <= max.x; x += size.x) {
			lines.push_back(vec3(x, min.y, z));
			lines.push_back(vec3(x, max.y, z));
			if (size.x == 0) {
				break;
			}
		}
		for (float y = min.y; y <= max.y; y += size.y) {
			lines.push_back(vec3(min.x, y, z));
			lines.push_back(vec3(max.x, y, z));
			if (size.x == 0) {
				break;
			}
		}
		if (size.x == 0) {
			break;
		}
	}

 	ShaderMgr* shader = ShaderMgr::Instance();
 	shader->Shape();
	shader->DrawShape(GL_LINES, &lines[0].x, lines.size(), color, true);
}

void DrawPoints(const std::vector<vec3>& points, d2d::Colorf color)
{
	ShaderMgr* shader = ShaderMgr::Instance();
	shader->Shape();
	shader->DrawShape(GL_POINTS, &points[0].x, points.size(), color, true);
}

}