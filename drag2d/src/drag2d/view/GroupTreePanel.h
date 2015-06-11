#ifndef _DRAG2D_GROUP_TREE_PANEL_H_
#define _DRAG2D_GROUP_TREE_PANEL_H_

#include "IViewPanel.h"

#include <wx/panel.h>
#include <json/json.h>

namespace d2d
{

class GroupTreeCtrl;
class MultiSpritesImpl;
class ViewPanelMgr;
class ISprite;

class GroupTreePanel : public wxPanel, public IViewPanel
{
public:
	GroupTreePanel(wxWindow* parent, MultiSpritesImpl* sprites_impl,
		ViewPanelMgr* view_panel_mgr);

	//
	//	interface IViewPanel
	//
	virtual void SelectSprite(ISprite* spr);
	virtual void ReorderSprite(ISprite* spr, bool up);

	void StoreToFile(Json::Value& value) const;
	void LoadFromFile(const Json::Value& value);

	void Remove(ISprite* sprite);

	void InitGroups(const std::map<std::string, std::vector<d2d::ISprite*> >& groups);

private:
	void InitLayout();
	
	wxSizer* InitButtonsLayout();

private:
	void OnAddPress(wxCommandEvent& event);
	void OnDelPress(wxCommandEvent& event);

private:
	MultiSpritesImpl* m_sprite_impl;

	GroupTreeCtrl* m_grouptree;

}; // GroupTreePanel

}

#endif // _DRAG2D_GROUP_TREE_PANEL_H_