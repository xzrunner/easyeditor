#ifndef _DRAG2D_STAGE_PANEL_H_
#define _DRAG2D_STAGE_PANEL_H_

#include <drag2d.h>

namespace ecomplex
{

class LibraryPanel;
class Symbol;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
		d2d::PropertySettingPanel* property, LibraryPanel* library,
		d2d::ViewPanelMgr* view_panel_mgr);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, Symbol* symbol, 
		d2d::PropertySettingPanel* property, LibraryPanel* library,
		d2d::ViewPanelMgr* view_panel_mgr);

	//
	// d2d::EditPanel interface
	//
	virtual void clear();

	//
	// d2d::MultiSpritesImpl interface
	//
	virtual void removeSprite(d2d::ISprite* sprite);
	virtual void insertSprite(d2d::ISprite* sprite);
	virtual bool resetSpriteOrder(d2d::ISprite* sprite, bool up);

	Symbol* getSymbol() { return m_symbol; }

private:
	Symbol* m_symbol;

	LibraryPanel* m_library;

	d2d::ViewPanelMgr* m_view_panel_mgr;

}; // StagePanel

}

#endif // _DRAG2D_STAGE_PANEL_H_