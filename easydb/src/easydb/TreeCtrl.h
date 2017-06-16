#ifndef _EASYDB_TREE_CTRL_H_
#define _EASYDB_TREE_CTRL_H_

#include <wx/treectrl.h>

#include <map>

namespace ee { class Sprite; }

namespace edb
{

class Database;

class TreeCtrl : public wxTreeCtrl
{
public:
	TreeCtrl(wxWindow* parent);

	void Build(const Database& db);

	void Clear();

private:
	enum
	{
		ID_CTRL
	};

private:
	void BuildFromNode(const Database& db, int node, wxTreeItemId parent, bool first);

//	void addNode(const Graphics& graph, const Node& node, wxTreeItemId parent);

	std::string GetItemName(const ee::Sprite& spr) const;

	void OnSelChanged(wxTreeEvent& event);
	void OnItemClick(wxTreeEvent& event);

private:
	wxTreeItemId m_root;

	std::map<wxTreeItemId, int> m_map2node;

	DECLARE_EVENT_TABLE()

}; // TreeCtrl

}

#endif // _EASYDB_TREE_CTRL_H_s