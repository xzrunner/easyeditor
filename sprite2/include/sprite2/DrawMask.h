#ifndef _SPRITE2_DRAW_MASK_H_
#define _SPRITE2_DRAW_MASK_H_

#include <SM_Matrix.h>

namespace s2
{

class Symbol;
class RenderParams;
class RenderColor;

class DrawMask
{
public:
	static void Draw(const Symbol* base, const Symbol* mask, const RenderParams& params);

private:
	static void DrawBaseToFbo0(const Symbol* base, const RenderColor& rc);
	static void DrawMaskToFbo1(const Symbol* mask);
	static void DrawMashFromFbo(const Symbol* mask, const sm::mat4& mt);

}; // DrawMask

}

#endif // _SPRITE2_DRAW_MASK_H_