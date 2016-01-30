#include "ReorderSpriteSJ.h"
#include "subject_id.h"

namespace d2d
{

ReorderSpriteSJ* ReorderSpriteSJ::m_instance = NULL;

ReorderSpriteSJ::ReorderSpriteSJ(int id)
	: Subject(id)
{
}

void ReorderSpriteSJ::Reorder(Sprite* spr, bool up, Observer* except)
{
	Params p;
	p.spr = spr;
	p.up = up;
	Notify((void*)&p, except);
}

ReorderSpriteSJ* ReorderSpriteSJ::Instance()
{
	if (!m_instance) {
		m_instance = new ReorderSpriteSJ(MSG_REORDER_SPRITE);
	}
	return m_instance;
}

}