#pragma once

#include <SM_Rect.h>
#include <SM_Matrix2D.h>

namespace pt2
{

class Texture;

class RenderSystem
{
public:
	static void DrawTexture(const Texture& tex, const sm::rect& pos, 
		const sm::Matrix2D& mat);

private:
	static bool CalcVertices(const sm::rect& pos, const sm::Matrix2D& mat, 
		float* vertices);

}; // RenderSystem

}