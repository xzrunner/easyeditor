#ifndef _EASYEDITOR_SPRITE_GAUSSIAN_BLUR_H_
#define _EASYEDITOR_SPRITE_GAUSSIAN_BLUR_H_

#include <SM_Matrix.h>

namespace ee
{

class Sprite;
class DTexC1;

class SpriteGaussianBlur
{
public:
	static void Draw(const Sprite* spr, const sm::mat4& mt);

private:
	static void DrawInit(const Sprite* spr, const sm::mat4& mt, DTexC1* fbo);
	static void DrawBetweenFBO(DTexC1* from, DTexC1* to, bool hori);
	static void DrawToScreen(DTexC1* fbo, const sm::vec2& offset);

}; // SpriteGaussianBlur

}

#endif // _EASYEDITOR_SPRITE_GAUSSIAN_BLUR_H_