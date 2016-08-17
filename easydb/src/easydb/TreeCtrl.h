#ifndef _EASYDB_TREE_CTRL_H_
#define _EASYDB_TREE_CTRL_H_

#include <wx/treectrl.h>

#include <map>

namespace ee { class Sprite; }

namespace edb
{

class Graphics;
struct Node;

class TreeCtrl : public wxTreeCtrl
{
public:
	TreeCtrl(wxWindow* parent);

	void test();

	void init(const Graphics& graph);
	void init(const wxArrayString& files);

	void clear();

private:
	enum
	{
		ID_CTRL
	};

private:
	void addNode(const Graphics& graph, const Node& node, wxTreeItemId parent);

	std::string getItemName(const ee::Sprite& spr) const;

	void onSelChanged(wxTreeEvent& event);
	void onItemClick(wxTreeEvent& event);

	ee::Sprite* querySpriteByID(wxTreeItemId id) const;

private:
	wxTreeItemId m_root;

	std::map<wxTreeItemId, ee::Sprite*> m_mapID2Sprite;

	std::map<wxTreeItemId, std::string> m_mapID2Path;

	DECLARE_EVENT_TABLE()

}; // TreeCtrl

}

#endif // _EASYDB_TREE_CTRL_H_s