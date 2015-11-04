#include "RemoveSpriteSJ.h"
#include "subject_id.h"

namespace d2d
{

RemoveSpriteSJ* RemoveSpriteSJ::m_instance = NULL;

RemoveSpriteSJ::RemoveSpriteSJ(int id)
	: Subject(id)
{
}

void RemoveSpriteSJ::Remove(ISprite* spr, Observer* except)
{
	Notify((void*)spr, except);
}

RemoveSpriteSJ* RemoveSpriteSJ::Instance()
{
	if (!m_instance) {
		m_instance = new RemoveSpriteSJ(REMOVE_SPRITE);
	}
	return m_instance;
}

}