#include "SpriteSelectedSJ.h"
#include "subject_id.h"

namespace d2d
{

SpriteSelectedSJ* SpriteSelectedSJ::m_instance = NULL;

SpriteSelectedSJ::SpriteSelectedSJ(int id)
	: Subject(id)
{
}

void SpriteSelectedSJ::OnSelected(const Params& p, Observer* except)
{
	Notify((void*)&p, except);
}

SpriteSelectedSJ* SpriteSelectedSJ::Instance()
{
	if (!m_instance) {
		m_instance = new SpriteSelectedSJ(SPRITE_SELECTED);
	}
	return m_instance;
}

}