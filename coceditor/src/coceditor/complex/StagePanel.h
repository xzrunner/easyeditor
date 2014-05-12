#pragma once

#include <drag2d.h>

namespace coceditor
{
	class LibraryPanel;

namespace complex
{
	class StagePanel : public d2d::EditPanel, public d2d::MultiSpritesImpl
	{
	public:
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
			LibraryPanel* library, complex::Symbol* complexSymbol);
		virtual ~StagePanel();

		//
		// d2d::MultiSpritesImpl interface
		//
		virtual void traverseSprites(d2d::IVisitor& visitor, 
			d2d::TraverseType type = d2d::e_allExisting,
			bool order = true) const;
		virtual void removeSprite(d2d::ISprite* sprite) {}
		virtual void insertSprite(d2d::ISprite* sprite);
		virtual void clearSprites() {}

		virtual void resetSpriteOrder(d2d::ISprite* sprite, bool up);

		const LibraryPanel* getLibraryPanel() const { 
			return m_library; 
		}

//		void onDraw();

		void loadSymbol();
		void storeSymbol();

	private:
		class DragSymbolTarget : public wxTextDropTarget
		{
		public:
			DragSymbolTarget(StagePanel* stage, LibraryPanel* library);

			virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

		private:
			StagePanel* m_stage;
			LibraryPanel* m_library;

		}; // DragTileTarget

	private:
		LibraryPanel* m_library;

		complex::Symbol* m_complexSymbol;

	}; // StagePanel
}
}

