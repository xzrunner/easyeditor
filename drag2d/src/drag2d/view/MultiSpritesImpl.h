#pragma once

#include <wx/wx.h>

#include "DataTraverseType.h"

#include "common/visitors.h"
#include "dataset/ObjSelectionSet.h"

namespace d2d
{
	class MultiSpritesImpl
	{
	public:
		MultiSpritesImpl(wxWindow* wnd);
		virtual ~MultiSpritesImpl();

		virtual void traverseSprites(IVisitor& visitor, 
			DataTraverseType type = DT_ALL, bool order = true) const = 0;
		virtual void removeSprite(ISprite* sprite) = 0;
		virtual void insertSprite(ISprite* sprite) = 0;
		virtual void clearSprites() = 0;

		virtual void resetSpriteOrder(ISprite* sprite, bool up) = 0;

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
			virtual void visit(Object* object, bool& bFetchNext);

		private:
			const Vector& m_pos;
			ISprite** m_pResult;

		}; // PointQueryVisitor

		class RectQueryVisitor : public IVisitor
		{
		public:
			RectQueryVisitor(const Rect& rect, bool contain, std::vector<ISprite*>& result);
			virtual void visit(Object* object, bool& bFetchNext);

		private:
			const Rect& m_rect;
			bool m_contain;
			std::vector<ISprite*>& m_result;

		}; // RectQueryVisitor

		class RemoveSelectionVisitor : public IVisitor
		{
		public:
			RemoveSelectionVisitor(MultiSpritesImpl* spritesImpl);
			virtual void visit(Object* object, bool& bFetchNext);

		private:
			MultiSpritesImpl* m_spritesImpl;

		}; // RemoveSelectionVisitor

	protected:
		SpriteSelection* m_spriteSelection;

	private:
		wxWindow* m_wnd;

	}; // MultiSpritesImpl
}

