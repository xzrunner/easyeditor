#pragma once

#include "interfaces.h"
#include "PasteSymbolOP.h"

#include "component/PasteSymbolOffsetCMPT.h"

namespace d2d
{
	class MultiSpritesImpl;
	class LibraryPanel;
	class Sprite;

	class PasteSymbolTileOP : public PasteSymbolOP
	{
	public:
		PasteSymbolTileOP(wxWindow* wnd, EditPanelImpl* stage, MultiSpritesImpl* spritesImpl, 
			LibraryPanel* libraryPanel, PasteSymbolOffsetCMPT<PasteSymbolTileOP>* cmpt);

		virtual bool OnMouseLeftDown(int x, int y);
		virtual bool OnMouseRightDown(int x, int y);
		virtual bool OnMouseMove(int x, int y);

		virtual bool OnDraw() const;

	private:
		class NearestQueryVisitor : public Visitor
		{
		public:
			NearestQueryVisitor(const Vector& pos, Sprite** ret);

			virtual void Visit(Object* object, bool& next);

		private:
			const Vector& m_pos;

			float m_dis;

			Sprite** m_result;

		}; // NearestQueryVisitor

	private:
		MultiSpritesImpl* m_spritesImpl;

		PasteSymbolOffsetCMPT<PasteSymbolTileOP>* m_cmpt;

		bool m_bCaptured;

		float m_rotate;

	}; // PasteSymbolTileOP
}

