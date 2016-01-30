#ifndef _EASYEDITOR_SPRITE_BLEND_H_
#define _EASYEDITOR_SPRITE_BLEND_H_

namespace ee
{

class Sprite;
class Matrix;

class SpriteBlend
{
public:
	void Draw(const Sprite* sprite, const Matrix& mt) const;
	
	static SpriteBlend* Instance();
		
private:
	SpriteBlend();

private:
	void DrawSprToTmp(const Sprite* sprite, const Matrix& mt) const;
	void DrawTmpToScreen(const Sprite* sprite, const Matrix& mt) const;

private:
	static SpriteBlend* m_instance;

}; // SpriteBlend

}

#endif // _EASYEDITOR_SPRITE_BLEND_H_