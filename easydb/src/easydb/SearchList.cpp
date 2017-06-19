#include "SearchList.h"
#include "Database.h"
#include "MainList.h"
#include "LeafNode.h"

namespace edb
{

BEGIN_EVENT_TABLE(SearchList, wxTreeCtrl)
	EVT_TREE_ITEM_ACTIVATED(ID_CTRL, SearchList::OnItemActivated)
END_EVENT_TABLE()

SearchList::SearchList(wxWindow* parent, MainList* main_list, const Database& db)
	: wxTreeCtrl(parent, ID_CTRL, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT | wxTR_HIDE_ROOT)
	, m_main_list(main_list)
	, m_db(db)
{
	SetBackgroundColour(wxColour(229, 229, 229));
}

void SearchList::OnSearch(const std::string& str)
{
	DeleteAllItems();

	m_root = AddRoot("search");

	static const int MAX = 50;

	std::vector<int> nodes;	
	m_db.QueryByStr(str, MAX, nodes);
	for (int i = 0, n = nodes.size(); i < n; ++i) 
	{
		const Node* node = m_db.Fetch(nodes[i]);
		if (node->Type() != NODE_LEAF) {
			continue;
		}
		const LeafNode* leaf = static_cast<const LeafNode*>(node);
		AppendItem(m_root, leaf->GetPath());
	}
}

void SearchList::OnItemActivated(wxTreeEvent& event)
{
	wxTreeItemId id = event.GetItem();
	wxString text = GetItemText(id);
	int node_id = m_db.QueryByPath(m_db.GetDirPath() + "\\" + text.ToStdString());
	m_main_list->OnSelected(node_id);
}

}