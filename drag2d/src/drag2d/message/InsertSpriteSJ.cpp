#include "InsertSpriteSJ.h"
#include "subject_id.h"

namespace d2d
{

InsertSpriteSJ* InsertSpriteSJ::m_instance = NULL;

InsertSpriteSJ::InsertSpriteSJ(int id)
	: Subject(id)
{
}

void InsertSpriteSJ::Insert(ISprite* spr, int idx = -1, Observer* except)
{
	Params p;
	p.spr = spr;
	p.idx = idx;
	Notify((void*)&p, except);
}

InsertSpriteSJ* InsertSpriteSJ::Instance()
{
	if (!m_instance) {
		m_instance = new InsertSpriteSJ(MSG_INSERT_SPRITE);
	}
	return m_instance;
}

}