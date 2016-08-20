#ifndef _SPRITE2_DRAW_GAUSSIAN_BLUR_H_
#define _SPRITE2_DRAW_GAUSSIAN_BLUR_H_

#include <SM_Matrix.h>

namespace s2
{

class Sprite;
class RenderParams;
class RenderColor;

class DrawGaussianBlur
{
public:
	static void Draw(const Sprite* spr, const RenderParams& params, int iterations);

	static void DrawToFbo0(const Sprite* spr, const RenderParams& params, int iterations);

	static void DrawToScreen(bool is_target0, const sm::vec2& offset);

private:
	static void DrawInit(const Sprite* spr, const RenderParams& params, bool is_target0);

	static void DrawBetweenFBO(bool is_t0_to_t1, bool hori, const RenderColor& col, float tex_size);

}; // DrawGaussianBlur

}

#endif // _SPRITE2_DRAW_GAUSSIAN_BLUR_H_