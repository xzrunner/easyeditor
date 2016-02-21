#include "primitive_draw.h"

#include "ShaderMgr.h"
#include "AABB.h"

#include <gl/glew.h>

namespace e3d
{

void DrawLine(const ee::vec3& p0, const ee::vec3& p1, ee::Colorf color)
{
	ee::vec3 vertices[] = {p0, p1};

	ShaderMgr* shader = ShaderMgr::Instance();
	shader->Shape();
	shader->DrawShape(GL_LINES, &vertices[0].x, 2, color, true);
}

void DrawTriLine(const ee::vec3& p0, const ee::vec3& p1, const ee::vec3& p2, ee::Colorf color)
{
	ee::vec3 vertices[] = {p0, p1, p2};

	ShaderMgr* shader = ShaderMgr::Instance();
	shader->Shape();
	shader->DrawShape(GL_LINE_LOOP, &vertices[0].x, 3, color, true);
}

void DrawCube(const AABB& aabb, ee::Colorf color)
{
	DrawCube(aabb.Min(), aabb.Max(), color);
}

void DrawCube(const ee::mat4& mat, const AABB& aabb, ee::Colorf color)
{
	const ee::vec3& min = aabb.Min();
	const ee::vec3& max = aabb.Max();

	ee::vec3 vertices[8];
	vertices[0] = mat * min; 
	vertices[1] = mat * ee::vec3(max.x, min.y, min.z);
	vertices[2] = mat * ee::vec3(max.x, max.y, min.z);
	vertices[3] = mat * ee::vec3(min.x, max.y, min.z);
	vertices[4] = mat * ee::vec3(min.x, min.y, max.z);
	vertices[5] = mat * ee::vec3(max.x, min.y, max.z);
	vertices[6] = mat * max;
	vertices[7] = mat * ee::vec3(min.x, max.y, max.z);

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

void DrawCube(const ee::vec3& min, const ee::vec3& max, ee::Colorf color)
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

void DrawCube(const ee::vec3& min, const ee::vec3& max, int texid)
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

void DrawCross(const ee::vec3& center, const ee::vec3& size, ee::Colorf color)
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

void DrawGrids(const ee::vec3& min, const ee::vec3& max, const ee::vec3& size, ee::Colorf color)
{
	std::vector<ee::vec3> lines;
	int cx = size.x ? ceil(max.x - min.x) / size.x : 1;
	int cy = size.y ? ceil(max.y - min.y) / size.y : 1;
	int cz = size.z ? ceil(max.z - min.z) / size.z : 1;
	lines.reserve(cx * cy * cz * 2);

	for (float z = min.z; z <= max.z; z += size.z) {
		for (float x = min.y; x <= max.x; x += size.x) {
			lines.push_back(ee::vec3(x, min.y, z));
			lines.push_back(ee::vec3(x, max.y, z));
			if (size.x == 0) {
				break;
			}
		}
		for (float y = min.y; y <= max.y; y += size.y) {
			lines.push_back(ee::vec3(min.x, y, z));
			lines.push_back(ee::vec3(max.x, y, z));
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

void DrawPoints(const std::vector<ee::vec3>& points, ee::Colorf color)
{
	ShaderMgr* shader = ShaderMgr::Instance();
	shader->Shape();
	shader->DrawShape(GL_POINTS, &points[0].x, points.size(), color, true);
}

}