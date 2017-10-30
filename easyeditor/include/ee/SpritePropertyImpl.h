#ifndef _EASYEDITOR_SPRITE_PROPERTY_IMPL_H_
#define _EASYEDITOR_SPRITE_PROPERTY_IMPL_H_

#include "Sprite.h"

namespace ee
{

class EditPanelImpl;

class SpritePropertyImpl
{
public:
	SpritePropertyImpl(EditPanelImpl* stage, const SprPtr& spr);

	void Translate(float x, float y);
	void Rotate(float angle);
	void Scale(float sx, float sy);
	void Shear(float kx, float ky);
	void Offset(float ox, float oy);
	void Mirror(bool mx, bool my);
	void Perspective(float px, float py);

	const SprPtr& GetSprite() { return m_spr; }

private:
	EditPanelImpl* m_stage;

	SprPtr m_spr;

}; // SpritePropertyImpl

}

#endif // _EASYEDITOR_SPRITE_PROPERTY_IMPL_H_