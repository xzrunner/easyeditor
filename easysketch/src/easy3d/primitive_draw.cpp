#include "primitive_draw.h"

#include "ShaderMgr.h"
#include "Cube.h"

namespace e3d
{

void DrawLine(const vec3& p0, const vec3& p1, d2d::Colorf color)
{
	float vertices[6];
	vertices[0] = p0.x;
	vertices[1] = p0.y;
	vertices[2] = p0.z;
	vertices[3] = p1.x;
	vertices[4] = p1.y;
	vertices[5] = p1.z;

	ShaderMgr* shader = ShaderMgr::Instance();
	shader->Shape();
	shader->SetShapeColor(color);
	shader->Commit();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glDrawArrays(GL_LINES, 0, 2);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawCube(const Cube& cube, d2d::Colorf color)
{
	vec3 min, max;
	cube.GetSize(min, max);
	DrawCube(min, max, color);
}

void DrawCube(const mat4& mat, const Cube& cube, d2d::Colorf color)
{
	float vertices[24];
	int idx = 0;

	vec3 min, max;
	cube.GetSize(min, max);

	vec3 pos = mat * min;
 	vertices[idx++] = pos.x;
 	vertices[idx++] = pos.y;
 	vertices[idx++] = pos.z;
 
	pos = mat * vec3(max.x, min.y, min.z);
	vertices[idx++] = pos.x;
	vertices[idx++] = pos.y;
	vertices[idx++] = pos.z;
 
	pos = mat * vec3(max.x, max.y, min.z);
	vertices[idx++] = pos.x;
	vertices[idx++] = pos.y;
	vertices[idx++] = pos.z;
 
	pos = mat * vec3(min.x, max.y, min.z);
	vertices[idx++] = pos.x;
	vertices[idx++] = pos.y;
	vertices[idx++] = pos.z;
 
	pos = mat * vec3(min.x, min.y, max.z);
	vertices[idx++] = pos.x;
	vertices[idx++] = pos.y;
	vertices[idx++] = pos.z;
 
	pos = mat * vec3(max.x, min.y, max.z);
	vertices[idx++] = pos.x;
	vertices[idx++] = pos.y;
	vertices[idx++] = pos.z;
 
	pos = mat * vec3(max.x, max.y, max.z);
	vertices[idx++] = pos.x;
	vertices[idx++] = pos.y;
	vertices[idx++] = pos.z;
 
	pos = mat * vec3(min.x, max.y, max.z);
	vertices[idx++] = pos.x;
	vertices[idx++] = pos.y;
	vertices[idx++] = pos.z;

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
	shader->SetShapeColor(color);
	shader->Commit();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, indices);

	glDisableClientState(GL_VERTEX_ARRAY);
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
	shader->SetShapeColor(color);
	shader->Commit();

 	glEnableClientState(GL_VERTEX_ARRAY);
 	glVertexPointer(3, GL_FLOAT, 0, vertices);
  
 	glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, indices);

 	glDisableClientState(GL_VERTEX_ARRAY);
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
	shader->SetShapeColor(color);
	shader->Commit();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glDrawArrays(GL_LINES, 0, 6);

	glDisableClientState(GL_VERTEX_ARRAY);
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
 	shader->SetShapeColor(color);
 	shader->Commit();
 
 	glEnableClientState(GL_VERTEX_ARRAY);
 	glVertexPointer(3, GL_FLOAT, 0, &lines[0].x);
 
 	glDrawArrays(GL_LINES, 0, lines.size());
 
 	glDisableClientState(GL_VERTEX_ARRAY);
}

}