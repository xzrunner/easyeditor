#ifndef _EASYUI_STAGE_PANEL_H_
#define _EASYUI_STAGE_PANEL_H_

#include <drag2d.h>

#include "dataset/SymbolsCfg.h"

namespace eui
{

class SymbolsCfg;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::PropertySettingPanel* property,
		d2d::LibraryPanel* library);

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

	void InitConfig();

	void SetViewlist(d2d::ViewlistPanel* viewlist) {
		m_viewlist = viewlist;
	}

private:
	SymbolsCfg m_symbols_cfg;

	d2d::ViewlistPanel* m_viewlist;

}; // StagePanel

}

#endif // _EASYUI_STAGE_PANEL_H_
