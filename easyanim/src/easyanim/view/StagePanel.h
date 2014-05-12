#pragma once

#include <wx/wx.h>
#include <wx/dnd.h>

#include <drag2d.h>

namespace eanim
{
	class Sprite;
	class KeyFrame;

	class StagePanel : public d2d::EditPanel, public d2d::MultiSpritesImpl
	{
	public:
		StagePanel(wxWindow* parent);

		//
		// d2d::EditPanel interface
		//
		virtual void clear();

		//
		// d2d::MultiSpritesImpl interface
		//
		virtual void traverseSprites(d2d::IVisitor& visitor, 
			d2d::TraverseType type = d2d::e_allExisting,
			bool order = true) const;
		virtual void removeSprite(d2d::ISprite* sprite);
		virtual void insertSprite(d2d::ISprite* sprite) {}
		virtual void clearSprites();

		virtual void resetSpriteOrder(d2d::ISprite* sprite, bool up) {}

		void insertSymbol(int index, wxCoord x, wxCoord y);
		void insertSkeleton(int index, wxCoord x, wxCoord y);
		void loadCurrFrameSprites();

		void resetCanvas();

	private:
		class DragSymbolTarget : public wxTextDropTarget
		{
		public:
			DragSymbolTarget(StagePanel* panel);

			virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

		private:
			StagePanel* m_panel;

		}; // DragTileTarget

	private:
		std::vector<Sprite*> m_sprites;

	}; // StagePanel
}

