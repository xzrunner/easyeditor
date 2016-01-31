#pragma once



namespace coceditor
{
	class LibraryPanel;

namespace complex
{
	class StagePanel : public ee::EditPanel, public ee::MultiSpritesImpl
	{
	public:
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
			LibraryPanel* library, complex::Symbol* complexSymbol);
		virtual ~StagePanel();

		//
		// ee::MultiSpritesImpl interface
		//
		virtual void traverseSprites(ee::IVisitor& visitor, 
			ee::TraverseType type = ee::e_allExisting,
			bool order = true) const;
		virtual void removeSprite(ee::ISprite* sprite) {}
		virtual void insertSprite(ee::ISprite* sprite);
		virtual void clearSprites() {}

		virtual void resetSpriteOrder(ee::ISprite* sprite, bool up);

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

