#include "sprite_msg.h"
#include "subject_id.h"
#include "ScriptsSprite.h"

namespace ee
{

SUBJECT_DEFINITION(ClearSpriteSJ, MSG_CLEAR_SPRITE)
void ClearSpriteSJ::Clear(Observer* except)
{
	Notify(NULL, except);
}

SUBJECT_DEFINITION(InsertSpriteSJ, MSG_INSERT_SPRITE)
void InsertSpriteSJ::Insert(Sprite* spr, int idx, Observer* except)
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

SUBJECT_DEFINITION(QuerySelectedSpriteLayerSJ, MSG_QUERY_SPR_LAYER)
int QuerySelectedSpriteLayerSJ::Query()
{
	int layer = INT_MAX - 1;
	Notify(&layer);
	return layer;
}

SUBJECT_DEFINITION(RemoveSpriteSJ, MSG_REMOVE_SPRITE)
void RemoveSpriteSJ::Remove(Sprite* spr, Observer* except)
{
	Notify((void*)spr, except);
}

SUBJECT_DEFINITION(ReorderSpriteMostSJ, MSG_REORDER_SPRITE_MOST)
void ReorderSpriteMostSJ::Reorder(Sprite* spr, bool up, Observer* except)
{
	Params p;
	p.spr = spr;
	p.up = up;
	Notify((void*)&p, except);
}

SUBJECT_DEFINITION(ReorderSpriteSJ, MSG_REORDER_SPRITE)
void ReorderSpriteSJ::Reorder(Sprite* spr, bool up, Observer* except)
{
	Params p;
	p.spr = spr;
	p.up = up;
	Notify((void*)&p, except);
}

SUBJECT_DEFINITION(SelectSpriteSetSJ, MSG_SELECT_SPRITE_SET)
void SelectSpriteSetSJ::Select(const SpriteSelection* selection, Observer* except)
{
	Notify((void*)selection, except);
}

SUBJECT_DEFINITION(SelectSpriteSJ, MSG_SELECT_SPRITE)
void SelectSpriteSJ::Select(Sprite* spr, bool clear, Observer* except)
{
	Params p;
	p.spr = spr;
	p.clear = clear;
	Notify((void*)&p, except);
}

SUBJECT_DEFINITION(SpriteNameChangeSJ, MSG_SPRITE_NAME_CHANGE)
void SpriteNameChangeSJ::OnSpriteNameChanged(const Sprite* spr, Observer* except)
{
	Notify((void*)spr, except);
}

}