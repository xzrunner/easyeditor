#include "primitive_draw.h"

#include "ShaderMgr.h"
#include "Cube.h"

namespace e3d
{

void DrawCube(const Cube& cube, d2d::Colorf color)
{
	vec3 min, max;
	cube.GetSize(min, max);
	DrawCube(min, max, color);
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
	shader->SetShapeColor(d2d::LIGHT_RED);
	shader->Commit();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glDrawArrays(GL_LINES, 0, 6);

	glDisableClientState(GL_VERTEX_ARRAY);
}

}