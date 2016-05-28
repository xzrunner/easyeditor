#ifndef _EASYEDITOR_SPRITE_GAUSSIAN_BLUR_H_
#define _EASYEDITOR_SPRITE_GAUSSIAN_BLUR_H_

#include <SM_Vector.h>

namespace s2 { class RenderParams; class RenderColor; }

namespace ee
{

class Sprite;
class DTexC1;

class SpriteGaussianBlur
{
public:
	static void Draw(const Sprite* spr, const s2::RenderParams& params);

	static void DrawToFbo0(const Sprite* spr, const s2::RenderParams& params);

	static void DrawToScreen(DTexC1* fbo, const sm::vec2& offset);

private:
	static void DrawInit(const Sprite* spr, const s2::RenderParams& params, DTexC1* fbo);

	static void DrawBetweenFBO(DTexC1* from, DTexC1* to, bool hori, const s2::RenderColor& col);

}; // SpriteGaussianBlur

}

#endif // _EASYEDITOR_SPRITE_GAUSSIAN_BLUR_H_