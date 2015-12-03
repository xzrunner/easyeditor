#include "InsertSpriteSJ.h"
#include "subject_id.h"

#include "dataset/ScriptsSprite.h"
#include "message/QuerySelectedSpriteLayerSJ.h"

namespace d2d
{

InsertSpriteSJ* InsertSpriteSJ::m_instance = NULL;

InsertSpriteSJ::InsertSpriteSJ(int id)
	: Subject(id)
{
}

void InsertSpriteSJ::Insert(ISprite* spr, int idx, Observer* except)
{
	if (dynamic_cast<ScriptsSprite*>(spr)) {
		return;
	}

	Params p;
	p.spr = spr;
	if (idx == -1) {
		p.idx = QuerySelectedSpriteLayerSJ::Instance()->Query() + 1;
	} else {
		p.idx = idx;
	}
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