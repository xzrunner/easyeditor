#ifndef _EASYUI_WINDOW_STAGE_PANEL_H_
#define _EASYUI_WINDOW_STAGE_PANEL_H_

#include "SymbolsCfg.h"
#include "AnchorMgr.h"
#include "UIStagePage.h"

#include <ee/SpritesPanelImpl.h>
#include <ee/CrossGuides.h>

namespace eui
{

class TopPannels;

namespace window
{

class SymbolsCfg;
class ToolbarPanel;

class StagePanel : public UIStagePage, public ee::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, TopPannels* top_pannels);

	//
	// UIStagePage interface
	//
	virtual void LoadFromFile(const char* filename);
	virtual void StoreToFile(const char* filename) const;
	virtual void EnablePage(bool enable);
	virtual void OnPreview();
	virtual void OnCode() const;

	void InitConfig();

	AnchorMgr* GetAnchorMgr() { return &m_anchor_mgr; }

	void SetViewSize(int width, int height);

	ee::CrossGuides& GetCrossGuides() { return m_guides; }

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	TopPannels* m_top_pannels;

	SymbolsCfg m_symbols_cfg;

	AnchorMgr m_anchor_mgr;

	int m_view_width, m_view_height;

	int m_toolbar_idx;
	ToolbarPanel* m_toolbar;

	ee::CrossGuides m_guides;

}; // StagePanel

}
}

#endif // _EASYUI_WINDOW_STAGE_PANEL_H_
