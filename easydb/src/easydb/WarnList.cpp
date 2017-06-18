#include "WarnList.h"
#include "Database.h"
#include "NodeType.h"
#include "LeafNode.h"

#include <map>

namespace edb
{

WarnList::WarnList(wxWindow* parent)
	: wxTreeCtrl(parent)
{
	SetBackgroundColour(wxColour(229, 229, 229));
}

void WarnList::Build(const Database& db)
{
	DeleteAllItems();

	m_root = AddRoot("warn");

	BuildDup(db);

	ExpandAll();
}

void WarnList::BuildDup(const Database& db)
{
	wxTreeItemId dup_id = AppendItem(m_root, "÷ÿ∏¥");

	const std::multimap<std::string, int>& map_md5 = db.GetMD5Map();
	if (map_md5.size() < 2) {
		return;
	}

	std::vector<int> dup_list;
	std::multimap<std::string, int>::const_iterator 
		itr0 = map_md5.begin(),
		itr1 = itr0;
	++itr1;
	for ( ; itr1 != map_md5.end(); ++itr0, ++itr1) 
	{
		if (itr0->first == itr1->first) {
			dup_list.push_back(itr0->second);
		} else if (!dup_list.empty()) {
			dup_list.push_back(itr0->second);
			InsertDupList(db, dup_list, dup_id);
			dup_list.clear();
		}
	}
	if (!dup_list.empty()) {
		InsertDupList(db, dup_list, dup_id);
	}
}

void WarnList::InsertDupList(const Database& db, const std::vector<int>& list, wxTreeItemId parent)
{
	wxTreeItemId id = AppendItem(parent, wxString::FromDouble(list.size()));
	for (int i = 0, n = list.size(); i < n; ++i) 
	{
		const Node* node = db.Fetch(list[i]);
		if (node->Type() == NODE_INDEX) {
			continue;
		}

		const LeafNode* leaf = static_cast<const LeafNode*>(node);
		AppendItem(id, leaf->GetPath());
	}
}

}