#ifndef _EASYEDITOR_SPRITE_BLEND_H_
#define _EASYEDITOR_SPRITE_BLEND_H_

#include <SM_Matrix.h>

namespace ee
{

class Sprite;

class SpriteBlend
{
public:
	static void Draw(const Sprite* spr, const sm::mat4& mt);
	
private:
	static void DrawSprToTmp(const Sprite* spr, const sm::mat4& mt);
	static void DrawTmpToScreen(const Sprite* spr, const sm::mat4& mt);

}; // SpriteBlend

}

#endif // _EASYEDITOR_SPRITE_BLEND_H_