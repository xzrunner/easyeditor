#ifndef _DRAG2D_SPRITE_PROPERTY_IMPL_H_
#define _DRAG2D_SPRITE_PROPERTY_IMPL_H_

namespace d2d
{

class EditPanelImpl;
class ISprite;

class SpritePropertyImpl
{
public:
	SpritePropertyImpl(EditPanelImpl* stage, ISprite* sprite);
	~SpritePropertyImpl();

	void Translate(float x, float y);
	void Rotate(float angle);
	void Scale(float sx, float sy);
	void Shear(float kx, float ky);
	void Offset(float ox, float oy);
	void Mirror(bool mx, bool my);
	void Perspective(float px, float py);

	ISprite* GetSprite() { return m_sprite; }

private:
	EditPanelImpl* m_stage;

	ISprite* m_sprite;

}; // SpritePropertyImpl

}

#endif // _DRAG2D_SPRITE_PROPERTY_IMPL_H_