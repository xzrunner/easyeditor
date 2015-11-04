#include "SelectSpriteSetSJ.h"
#include "subject_id.h"

namespace d2d
{

SelectSpriteSetSJ* SelectSpriteSetSJ::m_instance = NULL;

SelectSpriteSetSJ::SelectSpriteSetSJ(int id)
	: Subject(id)
{
}

void SelectSpriteSetSJ::OnSelected(const SpriteSelection* selection, Observer* except)
{
	Notify((void*)selection, except);
}

SelectSpriteSetSJ* SelectSpriteSetSJ::Instance()
{
	if (!m_instance) {
		m_instance = new SelectSpriteSetSJ(MULTI_SPRITE_SELECTED);
	}
	return m_instance;
}

}