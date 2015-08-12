#ifndef _EASYUI_STAGE_PANEL_H_
#define _EASYUI_STAGE_PANEL_H_

#include <drag2d.h>

#include "dataset/SymbolsCfg.h"
#include "dataset/AnchorMgr.h"

namespace eui
{

class SymbolsCfg;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::PropertySettingPanel* property,
		d2d::LibraryPanel* library,
		d2d::ViewPanelMgr* view_panel_mgr);

	//
	// d2d::EditPanel interface
	//
	virtual void Clear();

	//
	// d2d::MultiSpritesImpl interface
	//
	virtual bool InsertSprite(d2d::ISprite* sprite, int idx = -1);
	virtual bool RemoveSprite(d2d::ISprite* sprite);

	void InitConfig();

	AnchorMgr* GetAnchorMgr() { return &m_anchor_mgr; }

private:
	SymbolsCfg m_symbols_cfg;

	AnchorMgr m_anchor_mgr;

}; // StagePanel

}

#endif // _EASYUI_STAGE_PANEL_H_
