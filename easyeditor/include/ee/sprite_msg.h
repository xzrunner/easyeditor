#ifndef _EASYEDITOR_SPRITE_MSG_H_
#define _EASYEDITOR_SPRITE_MSG_H_

#include "Subject.h"

namespace ee
{

class ClearSpriteSJ : public Subject
{
public:
	void Clear(Observer* except = NULL);
	SUBJECT_DECLARATION(ClearSpriteSJ)
}; // ClearSpriteSJ

class Sprite;
class InsertSpriteSJ : public Subject
{
public:
	struct Params {
		Sprite* spr;
		int idx;
	};
public:
	void Insert(Sprite* spr, int idx = -1, Observer* except = NULL);
	SUBJECT_DECLARATION(InsertSpriteSJ)
}; // InsertSpriteSJ

class QuerySelectedSpriteLayerSJ : public Subject
{
public:
	int Query();
	SUBJECT_DECLARATION(QuerySelectedSpriteLayerSJ)
}; // QuerySelectedSpriteLayerSJ

class RemoveSpriteSJ : public Subject
{
public:
	void Remove(Sprite* spr, Observer* except = NULL);
	SUBJECT_DECLARATION(RemoveSpriteSJ)
}; // RemoveSpriteSJ

class ReorderSpriteMostSJ : public Subject
{
public:
	struct Params {
		Sprite* spr;
		bool up;
	};
public:
	void Reorder(Sprite* spr, bool up, Observer* except = NULL);
	SUBJECT_DECLARATION(ReorderSpriteMostSJ)
}; // ReorderSpriteMostSJ

class ReorderSpriteSJ : public Subject
{
public:
	struct Params {
		Sprite* spr;
		bool up;
	};
public:
	void Reorder(Sprite* spr, bool up, Observer* except = NULL);
	SUBJECT_DECLARATION(ReorderSpriteSJ)
}; // ReorderSpriteSJ

class SpriteSelection;
class SelectSpriteSetSJ : public Subject
{
public:
	void Select(const SpriteSelection* selection, Observer* except = NULL);
	SUBJECT_DECLARATION(SelectSpriteSetSJ)
}; // SelectSpriteSetSJ

class SelectSpriteSJ : public Subject
{
public:
	struct Params {
		Sprite* spr;
		bool clear;
	};
public:
	void Select(Sprite* spr, bool clear, Observer* except = NULL);
	SUBJECT_DECLARATION(SelectSpriteSJ)
}; // SelectSpriteSJ

class SpriteNameChangeSJ : public Subject
{
public:
	void OnSpriteNameChanged(const Sprite* spr, Observer* except = NULL);
	SUBJECT_DECLARATION(SpriteNameChangeSJ)
}; // SpriteNameChangeSJ

}

#endif // _EASYEDITOR_SPRITE_MSG_H_