#pragma once

#include <drag2d.h>

namespace complex
{
	class LibraryPanel;
	class Symbol;

	class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
	{
	public:
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
			d2d::PropertySettingPanel* property, LibraryPanel* library);
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame, Symbol* symbol, 
			d2d::PropertySettingPanel* property, LibraryPanel* library);

		//
		// d2d::EditPanel interface
		//
		virtual void clear();

		//
		// d2d::MultiSpritesImpl interface
		//
		virtual void removeSprite(d2d::ISprite* sprite);
		virtual void insertSprite(d2d::ISprite* sprite);

		Symbol* getSymbol() { return m_symbol; }

		void setViewlist(d2d::ViewlistPanel* viewlist) {
			m_viewlist = viewlist;
		}

	private:
		class DragSymbolTarget : public wxTextDropTarget
		{
		public:
			DragSymbolTarget(StagePanel* stage);

			virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

		private:
			StagePanel* m_stage;

		}; // DragTileTarget

	private:
		Symbol* m_symbol;

		d2d::ViewlistPanel* m_viewlist;

		LibraryPanel* m_library;

	}; // StagePanel
}

