#pragma once

#include <drag2d.h>

namespace ecomplex
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
		virtual void resetSpriteOrder(d2d::ISprite* sprite, bool up);

		Symbol* getSymbol() { return m_symbol; }

		void setViewlist(d2d::ViewlistPanel* viewlist) {
			m_viewlist = viewlist;
		}
		void SetGroupPanel(d2d::GroupTreePanel* group_panel) {
			m_group_panel = group_panel;
		}

	private:
		Symbol* m_symbol;

		LibraryPanel* m_library;

		d2d::ViewlistPanel* m_viewlist;

		d2d::GroupTreePanel* m_group_panel;

	}; // StagePanel
}

