#ifndef _DRAG2D_SPRITE_BLEND_H_
#define _DRAG2D_SPRITE_BLEND_H_

namespace d2d
{

class ISprite;
class Matrix;

class SpriteBlend
{
public:
	void Draw(const ISprite* sprite, const Matrix& mt) const;
	
	static SpriteBlend* Instance();
		
private:
	SpriteBlend();

private:
	static SpriteBlend* m_instance;

}; // SpriteBlend

}

#endif // _DRAG2D_SPRITE_BLEND_H_