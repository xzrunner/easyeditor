#ifndef _DRAG2D_SPRITE_BLEND2_H_
#define _DRAG2D_SPRITE_BLEND2_H_

namespace d2d
{

class ISprite;
class Matrix;

class SpriteBlend2
{
public:
	void Draw(const ISprite* sprite, const Matrix& mt) const;
	
	static SpriteBlend2* Instance();
		
private:
	SpriteBlend2();

private:
	mutable int m_blend_idx;

private:
	static SpriteBlend2* m_instance;

}; // SpriteBlend2

}

#endif // _DRAG2D_SPRITE_BLEND2_H_