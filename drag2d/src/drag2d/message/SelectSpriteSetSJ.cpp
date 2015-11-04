#include "SelectSpriteSetSJ.h"
#include "subject_id.h"

namespace d2d
{

SelectSpriteSetSJ* SelectSpriteSetSJ::m_instance = NULL;

SelectSpriteSetSJ::SelectSpriteSetSJ(int id)
	: Subject(id)
{
}

void SelectSpriteSetSJ::Select(const SpriteSelection* selection, Observer* except)
{
	Notify((void*)selection, except);
}

SelectSpriteSetSJ* SelectSpriteSetSJ::Instance()
{
	if (!m_instance) {
		m_instance = new SelectSpriteSetSJ(SELECT_SPRITE_SET);
	}
	return m_instance;
}

}