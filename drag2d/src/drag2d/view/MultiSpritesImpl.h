#ifndef _DRAG2D_MULTI_SPRITES_IMPL_H_
#define _DRAG2D_MULTI_SPRITES_IMPL_H_

#include "ISpriteViewPanel.h"
#include "DataTraverseType.h"

#include "common/visitors.h"

#include <wx/wx.h>

namespace d2d
{

class EditPanelImpl;
class SpriteSelection;
class Rect;

class MultiSpritesImpl : public ISpriteViewPanel
{
public:
	MultiSpritesImpl(EditPanelImpl* stage);
	virtual ~MultiSpritesImpl();

	//
	//	interface ISpriteViewPanel
	//
	virtual void SelectSprite(ISprite* spr, bool clear);
	virtual void SelectMultiSprites(SpriteSelection* selection);
	virtual bool ReorderSprite(d2d::ISprite* sprite, bool up);
	virtual bool InsertSprite(ISprite* sprite);
	virtual bool RemoveSprite(ISprite* sprite);
	virtual bool ClearAllSprite();

	virtual void TraverseSprites(IVisitor& visitor, 
		DataTraverseType type = DT_ALL, bool order = true) const = 0;

	virtual ISprite* QuerySpriteByPos(const Vector& pos) const;
	virtual void QuerySpritesByRect(const Rect& rect, bool contain, std::vector<ISprite*>& result) const;		

	SpriteSelection* GetSpriteSelection() { return m_sprite_selection; }
	void ClearSpriteSelection();

private:
	class PointQueryVisitor : public IVisitor
	{
	public:
		PointQueryVisitor(const Vector& pos, ISprite** pResult);
		virtual void Visit(Object* object, bool& bFetchNext);

	private:
		const Vector& m_pos;
		ISprite** m_pResult;

	}; // PointQueryVisitor

	class RectQueryVisitor : public IVisitor
	{
	public:
		RectQueryVisitor(const Rect& rect, bool contain, std::vector<ISprite*>& result);
		virtual void Visit(Object* object, bool& bFetchNext);

	private:
		const Rect& m_rect;
		bool m_contain;
		std::vector<ISprite*>& m_result;

	}; // RectQueryVisitor

protected:
	SpriteSelection* m_sprite_selection;

}; // MultiSpritesImpl

}

#endif // _DRAG2D_MULTI_SPRITES_IMPL_H_