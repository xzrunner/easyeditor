#pragma once

#include "interfaces.h"
#include "PasteSymbolOP.h"

#include "component/PasteSymbolOffsetCMPT.h"

namespace d2d
{
	class MultiSpritesImpl;
	class LibraryPanel;
	class ISprite;

	class PasteSymbolTileOP : public PasteSymbolOP
	{
	public:
		PasteSymbolTileOP(wxWindow* wnd, d2d::EditPanelImpl* stage, MultiSpritesImpl* spritesImpl, 
			LibraryPanel* libraryPanel, PasteSymbolOffsetCMPT<PasteSymbolTileOP>* cmpt);

		virtual bool OnMouseLeftDown(int x, int y);
		virtual bool OnMouseRightDown(int x, int y);
		virtual bool OnMouseMove(int x, int y);

		virtual bool OnDraw() const;

	private:
		class NearestQueryVisitor : public IVisitor
		{
		public:
			NearestQueryVisitor(const Vector& pos, ISprite** ret);

			virtual void Visit(Object* object, bool& bFetchNext);

		private:
			const Vector& m_pos;

			float m_dis;

			ISprite** m_result;

		}; // NearestQueryVisitor

	private:
		PasteSymbolOffsetCMPT<PasteSymbolTileOP>* m_cmpt;

		bool m_bCaptured;

		float m_rotate;

	}; // PasteSymbolTileOP
}

