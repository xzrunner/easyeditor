#ifndef _DRAG2D_GROUP_TREE_PANEL_H_
#define _DRAG2D_GROUP_TREE_PANEL_H_

#include "ISpriteViewPanel.h"

#include <wx/panel.h>
#include <json/json.h>

namespace d2d
{

class GroupTreeCtrl;
class MultiSpritesImpl;
class ViewPanelMgr;
class ISprite;

class GroupTreePanel : public wxPanel, public ISpriteViewPanel
{
public:
	GroupTreePanel(wxWindow* parent, MultiSpritesImpl* sprites_impl,
		ViewPanelMgr* view_panel_mgr);

	//
	//	interface ISpriteViewPanel
	//
	virtual void SelectSprite(ISprite* spr, bool clear);
	virtual void SelectMultiSprites(SpriteSelection* selection);
	virtual bool ReorderSprite(ISprite* spr, bool up);
	virtual bool InsertSprite(ISprite* spr);
	virtual bool RemoveSprite(ISprite* spr);
	virtual bool ClearAllSprite() { return false; }

	void StoreToFile(Json::Value& value) const;
	void LoadFromFile(const Json::Value& value);

	void Remove(ISprite* sprite);

	void InitGroups(const std::map<std::string, std::vector<d2d::ISprite*> >& groups);

	void Clear();

private:
	void InitLayout();
	
	void InitTitleLayout(wxSizer* sizer);
	wxSizer* InitHeaderLayout();

private:
	void OnAddPress(wxCommandEvent& event);
	void OnDelPress(wxCommandEvent& event);

private:
	MultiSpritesImpl* m_sprite_impl;

	GroupTreeCtrl* m_grouptree;

}; // GroupTreePanel

}

#endif // _DRAG2D_GROUP_TREE_PANEL_H_