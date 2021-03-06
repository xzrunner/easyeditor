#ifndef _EASYEDITOR_MULTI_SPRITES_IMPL_H_
#define _EASYEDITOR_MULTI_SPRITES_IMPL_H_

#include "DataTraverseType.h"
#include "Observer.h"
#include "Visitor.h"
#include "Sprite.h"

#include <SM_Vector.h>
#include <SM_Rect.h>

#include <wx/wx.h>

namespace ee
{

class EditPanelImpl;
class SpriteSelection;

class MultiSpritesImpl : public Observer
{
public:
	MultiSpritesImpl(EditPanelImpl* stage);
	virtual ~MultiSpritesImpl();

	virtual void TraverseSprites(RefVisitor<Sprite>& visitor,
		DataTraverseType type = DT_ALL, bool order = true) const = 0;

	virtual ee::SprPtr QuerySpriteByPos(const sm::vec2& pos) const;
	virtual void QuerySpritesByRect(const sm::rect& rect, bool contain, std::vector<ee::SprPtr>& result) const;

	SpriteSelection* GetSpriteSelection() { return m_sprite_selection; }
	void ClearSelectedSprite();

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	void OnSpriteSelected(const SprPtr& spr, bool clear);

protected:
	SpriteSelection* m_sprite_selection;

}; // MultiSpritesImpl

}

#endif // _EASYEDITOR_MULTI_SPRITES_IMPL_H_