#include "SpriteNameChangeSJ.h"
#include "subject_id.h"

namespace d2d
{

SpriteNameChangeSJ* SpriteNameChangeSJ::m_instance = NULL;

SpriteNameChangeSJ::SpriteNameChangeSJ(int id)
	: Subject(id)
{
}

void SpriteNameChangeSJ::OnSpriteNameChanged(const Sprite* spr, Observer* except)
{
	Notify((void*)spr, except);
}

SpriteNameChangeSJ* SpriteNameChangeSJ::Instance()
{
	if (!m_instance) {
		m_instance = new SpriteNameChangeSJ(MSG_SPRITE_NAME_CHANGE);
	}
	return m_instance;
}

}