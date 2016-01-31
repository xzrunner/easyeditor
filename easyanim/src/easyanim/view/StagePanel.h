#pragma once

#include <wx/wx.h>
#include <wx/dnd.h>



namespace eanim
{
	class Sprite;
	class KeyFrame;

	class StagePanel : public ee::EditPanel, public ee::MultiSpritesImpl
	{
	public:
		StagePanel(wxWindow* parent);

		//
		// ee::EditPanel interface
		//
		virtual void clear();

		//
		// ee::MultiSpritesImpl interface
		//
		virtual void traverseSprites(ee::IVisitor& visitor, 
			ee::TraverseType type = ee::e_allExisting,
			bool order = true) const;
		virtual void removeSprite(ee::ISprite* sprite);
		virtual void insertSprite(ee::ISprite* sprite) {}
		virtual void clearSprites();

		virtual void resetSpriteOrder(ee::ISprite* sprite, bool up) {}

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

