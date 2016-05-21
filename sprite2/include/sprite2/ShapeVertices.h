#ifndef _SPRITE2_SHAPE_VERTICES_H_
#define _SPRITE2_SHAPE_VERTICES_H_

#include <SM_Vector.h>
#include <SM_Matrix.h>

#include <vector>

namespace s2
{

class RenderColor;

class ShapeVertices
{
public:
	static void Draw(const std::vector<sm::vec2>& vertices, bool closed,
		const sm::mat4& mt, const RenderColor& color);

}; // ShapeVertices

}

#endif // _SPRITE2_SHAPE_VERTICES_H_