#ifndef _EASYUI_OVERALL_STAGE_PANEL_H_
#define _EASYUI_OVERALL_STAGE_PANEL_H_

#include <drag2d.h>

#include "SymbolsCfg.h"
#include "AnchorMgr.h"

#include "view/IUIStagePage.h"

namespace eui
{

class TopPannels;

namespace overall
{

class SymbolsCfg;

class StagePanel : public IUIStagePage, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, TopPannels* top_pannels);

	//
	// d2d::EditPanel interface
	//
	virtual void Clear();

	//
	// d2d::MultiSpritesImpl interface
	//
	virtual bool InsertSprite(d2d::ISprite* sprite, int idx = -1);
	virtual bool RemoveSprite(d2d::ISprite* sprite);

	//
	// IUIStagePage interface
	//
	virtual void OnSelected();
	virtual void LoadFromFile(const char* filename);
	virtual void StoreToFile(const char* filename) const;

	void InitConfig();

	AnchorMgr* GetAnchorMgr() { return &m_anchor_mgr; }

private:
	TopPannels* m_top_pannels;

	SymbolsCfg m_symbols_cfg;

	AnchorMgr m_anchor_mgr;

}; // StagePanel

}
}

#endif // _EASYUI_OVERALL_STAGE_PANEL_H_
