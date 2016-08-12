#ifndef _SPRITE2_SPRITE_RENDERER_H_
#define _SPRITE2_SPRITE_RENDERER_H_

#include "RenderParams.h"

#include <SM_Vector.h>

namespace s2
{

class Sprite;
class Symbol;

class SpriteRenderer
{
public:
	static void Draw(const Sprite* spr, const RenderParams& params = RenderParams());

	static void Draw(const Symbol* sym, 
		             const RenderParams& params = RenderParams(),
					 const sm::vec2& pos = sm::vec2(0, 0),
					 float angle = 0,
					 const sm::vec2& scale = sm::vec2(1, 1),
					 const sm::vec2& shear = sm::vec2(0, 0));

}; // SpriteRenderer

}

#endif // _SPRITE2_SPRITE_RENDERER_H_