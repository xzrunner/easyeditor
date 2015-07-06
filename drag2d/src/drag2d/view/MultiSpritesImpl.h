#ifndef _DRAG2D_MULTI_SPRITES_IMPL_H_
#define _DRAG2D_MULTI_SPRITES_IMPL_H_

#include "ISpriteViewPanel.h"
#include "DataTraverseType.h"

#include "common/visitors.h"
#include "dataset/ObjSelectionSet.h"

#include <wx/wx.h>

namespace d2d
{

class MultiSpritesImpl : public ISpriteViewPanel
{
public:
	MultiSpritesImpl(wxWindow* wnd);
	virtual ~MultiSpritesImpl();

	//
	//	interface ISpriteViewPanel
	//
	virtual void SelectSprite(ISprite* spr, bool clear);
	virtual void SelectMultiSprites(SpriteSelection* selection);
	virtual void ReorderSprite(ISprite* spr, bool up);
	virtual void InsertSprite(ISprite* spr);
	virtual void RemoveSprite(ISprite* spr);

	virtual void traverseSprites(IVisitor& visitor, 
		DataTraverseType type = DT_ALL, bool order = true) const = 0;
	virtual void removeSprite(ISprite* sprite) = 0;
	virtual void insertSprite(ISprite* sprite) = 0;
	virtual void clearSprites() = 0;
	virtual bool resetSpriteOrder(ISprite* sprite, bool up) = 0;

	virtual ISprite* querySpriteByPos(const Vector& pos) const;
	virtual void querySpritesByRect(const Rect& rect, bool contain, std::vector<ISprite*>& result) const;		

	SpriteSelection* getSpriteSelection() { return m_spriteSelection; }
	void removeSpriteSelection();

protected:
	void refresh();

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

	class RemoveSelectionVisitor : public IVisitor
	{
	public:
		RemoveSelectionVisitor(MultiSpritesImpl* spritesImpl);
		virtual void Visit(Object* object, bool& bFetchNext);

	private:
		MultiSpritesImpl* m_spritesImpl;

	}; // RemoveSelectionVisitor

protected:
	SpriteSelection* m_spriteSelection;

private:
	wxWindow* m_wnd;

}; // MultiSpritesImpl

}

#endif // _DRAG2D_MULTI_SPRITES_IMPL_H_