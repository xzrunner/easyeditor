#include "MultiSpriteSelectedSJ.h"
#include "subject_id.h"

namespace d2d
{

MultiSpriteSelectedSJ* MultiSpriteSelectedSJ::m_instance = NULL;

MultiSpriteSelectedSJ::MultiSpriteSelectedSJ(int id)
	: Subject(id)
{
}

void MultiSpriteSelectedSJ::OnSelected(const SpriteSelection* selection, Observer* except)
{
	Notify((void*)selection, except);
}

MultiSpriteSelectedSJ* MultiSpriteSelectedSJ::Instance()
{
	if (!m_instance) {
		m_instance = new MultiSpriteSelectedSJ(MULTI_SPRITE_SELECTED);
	}
	return m_instance;
}

}