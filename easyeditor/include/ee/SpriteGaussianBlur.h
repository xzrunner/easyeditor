#ifndef _EASYEDITOR_SPRITE_GAUSSIAN_BLUR_H_
#define _EASYEDITOR_SPRITE_GAUSSIAN_BLUR_H_

#include <SM_Vector.h>

namespace s2 { class RenderParams; class RenderColor; }

namespace ee
{

class Sprite;

class SpriteGaussianBlur
{
public:
	static void Draw(const Sprite* spr, const s2::RenderParams& params);

	static void DrawToFbo0(const Sprite* spr, const s2::RenderParams& params);

	static void DrawToScreen(bool is_target0, const sm::vec2& offset);

private:
	static void DrawInit(const Sprite* spr, const s2::RenderParams& params, bool is_target0);

	static void DrawBetweenFBO(bool is_t0_to_t1, bool hori, const s2::RenderColor& col, float tex_size);

}; // SpriteGaussianBlur

}

#endif // _EASYEDITOR_SPRITE_GAUSSIAN_BLUR_H_