#ifndef _EASYEDITOR_SPRITE_PROPERTY_IMPL_H_
#define _EASYEDITOR_SPRITE_PROPERTY_IMPL_H_

namespace ee
{

class EditPanelImpl;
class Sprite;

class SpritePropertyImpl
{
public:
	SpritePropertyImpl(EditPanelImpl* stage, Sprite* spr);
	~SpritePropertyImpl();

	void Translate(float x, float y);
	void Rotate(float angle);
	void Scale(float sx, float sy);
	void Shear(float kx, float ky);
	void Offset(float ox, float oy);
	void Mirror(bool mx, bool my);
	void Perspective(float px, float py);

	Sprite* GetSprite() { return m_spr; }

private:
	EditPanelImpl* m_stage;

	Sprite* m_spr;

}; // SpritePropertyImpl

}

#endif // _EASYEDITOR_SPRITE_PROPERTY_IMPL_H_