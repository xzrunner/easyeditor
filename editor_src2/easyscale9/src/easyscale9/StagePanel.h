#pragma once

#include <drag2d.h>

namespace escale9
{
	class Symbol;
	class ToolbarPanel;

	class StagePanel : public d2d::EditPanel, public d2d::MultiSpritesImpl
	{
	public:
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame, d2d::LibraryPanel* library);
		virtual ~StagePanel();

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
		virtual void insertSprite(d2d::ISprite* sprite);
		virtual void clearSprites();

		virtual void resetSpriteOrder(d2d::ISprite* sprite, bool up);

		Symbol* getPatchSymbol() { return m_patch; }

		d2d::ISprite* getSprite(int row, int col) {
			if (row < 0 || row >= 3 || col < 0 || col >= 3)
				return NULL;
			return m_sprites[row][col];
		}

		void rebuildPatchSymbol();

		void setToolbarPanel(ToolbarPanel* toolbar);

	private:
		bool isComplete() const;

	private:
		class DragSymbolTarget : public wxTextDropTarget
		{
		public:
			DragSymbolTarget(d2d::LibraryPanel* library, StagePanel* stage)
				: m_library(library), m_stage(stage) {}

			virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

		private:
			d2d::LibraryPanel* m_library;
			StagePanel* m_stage;

		}; // DragSymbolTarget

	private:
		// [2][0]
		// [1][0]
		// [0][0] [0][1] [0][2]
		d2d::ISprite* m_sprites[3][3];

		Symbol* m_patch;

		d2d::LibraryPanel* m_library;

		ToolbarPanel* m_toolbar;

	}; // StagePanel
}

