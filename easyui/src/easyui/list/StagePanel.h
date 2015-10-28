#ifndef _EASYUI_LIST_STAGE_PANEL_H_
#define _EASYUI_LIST_STAGE_PANEL_H_

#include <drag2d.h>

#include "UIList.h"

#include "view/IUIStagePage.h"

namespace eui
{

class TopPannels;

namespace list
{

class ToolbarPanel;

class StagePanel : public IUIStagePage, public d2d::MultiSpritesImpl
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
	virtual bool ReorderSprite(d2d::ISprite* sprite, bool up);
	virtual bool InsertSprite(d2d::ISprite* sprite, int idx = -1);
	virtual bool RemoveSprite(d2d::ISprite* sprite);
	virtual bool ClearAllSprite();
	virtual void TraverseSprites(d2d::IVisitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL, bool order = true) const;

	//
	// IUIStagePage interface
	//
	virtual void OnSelected();
	virtual void LoadFromFile(const char* filename);
	virtual void StoreToFile(const char* filename) const;

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