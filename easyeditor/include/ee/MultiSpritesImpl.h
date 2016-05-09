#ifndef _EASYEDITOR_MULTI_SPRITES_IMPL_H_
#define _EASYEDITOR_MULTI_SPRITES_IMPL_H_

#include "DataTraverseType.h"
#include "Observer.h"

#include <SM_Vector.h>

#include <wx/wx.h>

namespace ee
{

class EditPanelImpl;
class SpriteSelection;
class Rect;
class Sprite;
class Visitor;

class MultiSpritesImpl : public Observer
{
public:
	MultiSpritesImpl(EditPanelImpl* stage);
	virtual ~MultiSpritesImpl();

	virtual void TraverseSprites(Visitor& visitor, 
		DataTraverseType type = DT_ALL, bool order = true) const = 0;

	virtual Sprite* QuerySpriteByPos(const sm::vec2& pos) const;
	virtual void QuerySpritesByRect(const Rect& rect, bool contain, std::vector<Sprite*>& result) const;		

	SpriteSelection* GetSpriteSelection() { return m_sprite_selection; }
	void ClearSelectedSprite();

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void OnSpriteSelected(Sprite* spr, bool clear);

protected:
	SpriteSelection* m_sprite_selection;

}; // MultiSpritesImpl

}

#endif // _EASYEDITOR_MULTI_SPRITES_IMPL_H_