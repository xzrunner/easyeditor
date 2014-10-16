#include "PrimitiveDraw.h"
#include "ShaderMgr.h"
#include "Cube.h"

namespace e3d
{

void DrawCube(const Cube& cube)
{
	vec3 min, max;
	cube.GetSize(min, max);
	DrawCube(min, max);
}

void DrawCube(const vec3& min, const vec3& max)
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
	shader->Commit();

	//////////////////////////////////////////////////////////////////////////
 	glEnableClientState(GL_VERTEX_ARRAY);
 	glVertexPointer(3, GL_FLOAT, 0, vertices);
  
 	glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, indices);

 	glDisableClientState(GL_VERTEX_ARRAY);

	//////////////////////////////////////////////////////////////////////////
// 	glBegin(GL_LINE_STRIP);
// 	idx = 0;
// 	for (int i = 0; i < 8; ++i)
// 	{
// 		glVertex3f(vertices[idx++], vertices[idx++], vertices[idx++]);
// 	}
// 	glEnd();
}

}