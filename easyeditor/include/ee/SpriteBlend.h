#ifndef _EASYEDITOR_SPRITE_BLEND_H_
#define _EASYEDITOR_SPRITE_BLEND_H_

namespace ee
{

class Sprite;
class Matrix;

class SpriteBlend
{
public:
	static void Draw(const Sprite* sprite, const Matrix& mt);
	
private:
	static void DrawSprToTmp(const Sprite* sprite, const Matrix& mt);
	static void DrawTmpToScreen(const Sprite* sprite, const Matrix& mt);

}; // SpriteBlend

}

#endif // _EASYEDITOR_SPRITE_BLEND_H_