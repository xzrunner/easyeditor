#ifndef _SPRITE2_DRAW_OUTER_GLOW_H_
#define _SPRITE2_DRAW_OUTER_GLOW_H_

#include <SM_Matrix.h>

namespace s2
{

class Sprite;
class RenderParams;

class DrawOuterGlow
{
public:
	static void Draw(const Sprite* spr, const RenderParams& params, int iterations);

}; // DrawOuterGlow

}

#endif // _SPRITE2_DRAW_OUTER_GLOW_H_