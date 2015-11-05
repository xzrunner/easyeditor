#include "SelectSpriteSJ.h"
#include "subject_id.h"

namespace d2d
{

SelectSpriteSJ* SelectSpriteSJ::m_instance = NULL;

SelectSpriteSJ::SelectSpriteSJ(int id)
	: Subject(id)
{
}

void SelectSpriteSJ::Select(ISprite* spr, bool clear, Observer* except)
{
	Params p;
	p.spr = spr;
	p.clear = clear;
	Notify((void*)&p, except);
}

SelectSpriteSJ* SelectSpriteSJ::Instance()
{
	if (!m_instance) {
		m_instance = new SelectSpriteSJ(MSG_SELECT_SPRITE);
	}
	return m_instance;
}

}