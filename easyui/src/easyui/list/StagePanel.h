#ifndef _EASYUI_LIST_STAGE_PANEL_H_
#define _EASYUI_LIST_STAGE_PANEL_H_

#include <drag2d.h>

#include "UIList.h"
#include "UIStagePage.h"

namespace eui
{

class TopPannels;

namespace list
{

class ToolbarPanel;

class StagePanel : public UIStagePage, public d2d::MultiSpritesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, TopPannels* top_pannels);
	virtual ~StagePanel();

	//
	//	interface Observer
	//
	virtual void Notify(int sj_id, void* ud);

	//
	// d2d::MultiSpritesImpl interface
	//
	virtual void TraverseSprites(d2d::IVisitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL, bool order = true) const;

	//
	// UIStagePage interface
	//
	virtual void LoadFromFile(const char* filename);
	virtual void StoreToFile(const char* filename) const;
	virtual void EnablePage(bool enable);

	UIList& GetList() { return m_list; }

private:
	TopPannels* m_top_pannels;
	ToolbarPanel* m_toolbar;

	UIList m_list;

	int m_toolbar_idx;

}; // StagePanel

}
}

#endif // _EASYUI_LIST_STAGE_PANEL_H_