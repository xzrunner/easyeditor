#ifndef _EASYEDITOR_SPRITE_OUTER_GLOW_H_
#define _EASYEDITOR_SPRITE_OUTER_GLOW_H_

namespace s2 { class RenderParams; }

namespace ee
{

class Sprite;

class SpriteOuterGlow
{
public:
	static void Draw(const Sprite* spr, const s2::RenderParams& params, int iterations);

}; // SpriteOuterGlow

}

#endif // _EASYEDITOR_SPRITE_OUTER_GLOW_H_