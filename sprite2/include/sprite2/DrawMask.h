#ifndef _SPRITE2_DRAW_MASK_H_
#define _SPRITE2_DRAW_MASK_H_

#include <SM_Matrix.h>

namespace s2
{

class Sprite;
class RenderParams;
class RenderColor;

class DrawMask
{
public:
	static void Draw(const Sprite* base, const Sprite* mask, const RenderParams& params);

private:
	static void DrawBaseToFbo0(const Sprite* base, const RenderColor& rc);
	static void DrawMaskToFbo1(const Sprite* mask);
	static void DrawMashFromFbo(const Sprite* mask, const sm::mat4& mt);

}; // DrawMask

}

#endif // _SPRITE2_DRAW_MASK_H_