#ifndef _DRAG2D_GROUP_TREE_PANEL_H_
#define _DRAG2D_GROUP_TREE_PANEL_H_

#include <wx/panel.h>
#include <json/json.h>

namespace d2d
{

class GroupTreeCtrl;
class MultiSpritesImpl;
class ISprite;
class KeysState;

class GroupTreePanel : public wxPanel
{
public:
	GroupTreePanel(wxWindow* parent, MultiSpritesImpl* sprites_impl,
		const KeysState& key_state);
	virtual ~GroupTreePanel();

	void StoreToFile(Json::Value& value) const;
	void LoadFromFile(const Json::Value& value);

	void InitGroups(const std::map<std::string, std::vector<d2d::ISprite*> >& groups);

	void EnableExpand(bool enable);

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