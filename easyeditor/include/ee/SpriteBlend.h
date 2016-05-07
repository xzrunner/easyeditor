#ifndef _EASYEDITOR_SPRITE_BLEND_H_
#define _EASYEDITOR_SPRITE_BLEND_H_

#include <SM_Matrix.h>

namespace ee
{

class Sprite;

class SpriteBlend
{
public:
	static void Draw(const Sprite* sprite, const sm::mat4& mt);
	
private:
	static void DrawSprToTmp(const Sprite* sprite, const sm::mat4& mt);
	static void DrawTmpToScreen(const Sprite* sprite, const sm::mat4& mt);

}; // SpriteBlend

}

#endif // _EASYEDITOR_SPRITE_BLEND_H_