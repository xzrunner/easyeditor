#ifndef _POLYMESH_VERTEX_H_
#define _POLYMESH_VERTEX_H_

#include <SM_Vector.h>

namespace pm
{

class Vertex
{
public:
	sm::vec2 xy, uv;
	sm::vec2 ori_xy;
	
public:
	Vertex() {}
	Vertex(const sm::vec2& xy, const sm::vec2& uv) 
		: xy(xy), uv(uv), ori_xy(xy) {}

}; // Vertex

}

#endif // _POLYMESH_VERTEX_H_