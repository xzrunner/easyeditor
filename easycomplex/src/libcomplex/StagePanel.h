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
	virtual void Clear();

	//
	// d2d::MultiSpritesImpl interface
	//
	virtual bool ReorderSprite(d2d::ISprite* sprite, bool up);
	virtual bool InsertSprite(d2d::ISprite* sprite);
	virtual bool RemoveSprite(d2d::ISprite* sprite);

	Symbol* getSymbol() { return m_symbol; }

private:
	Symbol* m_symbol;

	LibraryPanel* m_library;

	d2d::ViewPanelMgr* m_view_panel_mgr;

}; // StagePanel

}

#endif // _DRAG2D_STAGE_PANEL_H_