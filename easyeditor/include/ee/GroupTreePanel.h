#ifndef _EASYEDITOR_GROUP_TREE_PANEL_H_
#define _EASYEDITOR_GROUP_TREE_PANEL_H_

#include <wx/panel.h>
#include <json/json.h>

namespace ee
{

class GroupTreeCtrl;
class MultiSpritesImpl;
class Sprite;
class KeysState;

class GroupTreePanel : public wxPanel
{
public:
	GroupTreePanel(wxWindow* parent, MultiSpritesImpl* sprites_impl,
		const KeysState& key_state);
	virtual ~GroupTreePanel();

	void StoreToFile(Json::Value& value) const;
	void LoadFromFile(const Json::Value& value);

	void InitGroups(const std::map<std::string, std::vector<Sprite*> >& groups);

	void EnableExpand(bool enable);

	void Clear();
	void Insert(Sprite* sprite);

private:
	void InitLayout();
	wxSizer* InitHeaderLayout();

private:
	void OnAddPress(wxCommandEvent& event);
	void OnDelPress(wxCommandEvent& event);

private:
	MultiSpritesImpl* m_sprite_impl;

	GroupTreeCtrl* m_grouptree;

}; // GroupTreePanel

}

#endif // _EASYEDITOR_GROUP_TREE_PANEL_H_