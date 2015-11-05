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
	virtual ~StagePanel();

	//
	//	interface Observer
	//
	virtual void Notify(int sj_id, void* ud);

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
