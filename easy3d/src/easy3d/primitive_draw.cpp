#include "primitive_draw.h"

#include "ShaderMgr.h"
#include "AABB.h"

#include <gl/glew.h>

namespace e3d
{

void DrawLine(const sm::vec3& p0, const sm::vec3& p1, ee::Colorf color)
{
	sm::vec3 vertices[] = {p0, p1};

	ShaderMgr* mgr = ShaderMgr::Instance();
	mgr->Shape();
	mgr->DrawShape(GL_LINES, &vertices[0].x, 2, color, true);
}

void DrawTriLine(const sm::vec3& p0, const sm::vec3& p1, const sm::vec3& p2, ee::Colorf color)
{
	sm::vec3 vertices[] = {p0, p1, p2};

	ShaderMgr* mgr = ShaderMgr::Instance();
	mgr->Shape();
	mgr->DrawShape(GL_LINE_LOOP, &vertices[0].x, 3, color, true);
}

void DrawCube(const AABB& aabb, ee::Colorf color)
{
	DrawCube(aabb.Min(), aabb.Max(), color);
}

void DrawCube(const sm::mat4& mat, const AABB& aabb, ee::Colorf color)
{
	const sm::vec3& min = aabb.Min();
	const sm::vec3& max = aabb.Max();

	sm::vec3 vertices[8];
	vertices[0] = mat * min; 
	vertices[1] = mat * sm::vec3(max.x, min.y, min.z);
	vertices[2] = mat * sm::vec3(max.x, max.y, min.z);
	vertices[3] = mat * sm::vec3(min.x, max.y, min.z);
	vertices[4] = mat * sm::vec3(min.x, min.y, max.z);
	vertices[5] = mat * sm::vec3(max.x, min.y, max.z);
	vertices[6] = mat * max;
	vertices[7] = mat * sm::vec3(min.x, max.y, max.z);

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

	ShaderMgr* mgr = ShaderMgr::Instance();
	mgr->Shape();
	mgr->DrawShape(GL_LINES, &vertices[0].x, 8, color, indices, 24);
}

void DrawCube(const sm::vec3& min, const sm::vec3& max, ee::Colorf color)
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

	ShaderMgr* mgr = ShaderMgr::Instance();
	mgr->Shape();
	mgr->DrawShape(GL_LINES, vertices, 8, color, indices, 24);
}

void DrawCube(const sm::vec3& min, const sm::vec3& max, int texid)
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

	e3d::ShaderMgr* mgr = e3d::ShaderMgr::Instance();
	mgr->Sprite();

	mgr->DrawTri(vertices, texcoords, 2 * 6 * 3, texid);
}

void DrawCross(const sm::vec3& center, const sm::vec3& size, ee::Colorf color)
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

	ShaderMgr* mgr = ShaderMgr::Instance();
	mgr->Shape();
	mgr->DrawShape(GL_LINES, vertices, 6, color, true);
}

void DrawGrids(const sm::vec3& min, const sm::vec3& max, const sm::vec3& size, ee::Colorf color)
{
	std::vector<sm::vec3> lines;
	int cx = size.x ? ceil(max.x - min.x) / size.x : 1;
	int cy = size.y ? ceil(max.y - min.y) / size.y : 1;
	int cz = size.z ? ceil(max.z - min.z) / size.z : 1;
	lines.reserve(cx * cy * cz * 2);

	for (float z = min.z; z <= max.z; z += size.z) {
		for (float x = min.y; x <= max.x; x += size.x) {
			lines.push_back(sm::vec3(x, min.y, z));
			lines.push_back(sm::vec3(x, max.y, z));
			if (size.x == 0) {
				break;
			}
		}
		for (float y = min.y; y <= max.y; y += size.y) {
			lines.push_back(sm::vec3(min.x, y, z));
			lines.push_back(sm::vec3(max.x, y, z));
			if (size.x == 0) {
				break;
			}
		}
		if (size.x == 0) {
			break;
		}
	}

 	ShaderMgr* mgr = ShaderMgr::Instance();
 	mgr->Shape();
	mgr->DrawShape(GL_LINES, &lines[0].x, lines.size(), color, true);
}

void DrawPoints(const std::vector<sm::vec3>& points, ee::Colorf color)
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	mgr->Shape();
	mgr->DrawShape(GL_POINTS, &points[0].x, points.size(), color, true);
}

}