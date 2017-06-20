#include "SearchList.h"
#include "Database.h"
#include "MainList.h"
#include "LeafNode.h"
#include "IndexNode.h"

#include <easyimage.h>

#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>
#include <gum/FilepathHelper.h>

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

void SearchList::OnSearchSameImages(float val)
{
	DeleteAllItems();

	m_root = AddRoot("search");

	const std::vector<Node*>& nodes = m_db.GetNodes();
	for (int i = 0, n = nodes.size(); i < n; ++i) 
	{
		if (nodes[i]->Type() != NODE_INDEX) {
			continue;
		}

		IndexNode* index = static_cast<IndexNode*>(nodes[i]);
		const std::vector<int>& children = index->GetChildren();
		std::vector<std::string> image_paths;
		for (int j = 0, m = children.size(); j < m; ++j)
		{
			const Node* cnode = m_db.Fetch(children[j]);
			if (cnode->Type() != NODE_LEAF) {
				continue;
			}
			const LeafNode* cleaf = static_cast<const LeafNode*>(cnode);
			std::string filepath = m_db.GetDirPath() + "\\" + cleaf->GetPath();
			if (ee::SymbolFile::Instance()->Type(filepath) == s2::SYM_IMAGE) {
				image_paths.push_back(filepath);
			}
		}
		if (image_paths.empty()) {
			continue;
		}

		std::vector<std::vector<int> > same;
		GetSameImages(image_paths, val, same);
		if (same.empty()) {
			continue;
		}
		
		for (int i = 0, n = same.size(); i < n; ++i) 
		{
			int num = same[i].size();
			wxTreeItemId id = AppendItem(m_root, wxString::FromDouble(num));
			for (int j = 0; j < num; ++j) 
			{
				std::string relative = gum::FilepathHelper::Relative(m_db.GetDirPath(), image_paths[same[i][j]]);
				AppendItem(id, relative);
			}
		}
	}
}

void SearchList::OnItemActivated(wxTreeEvent& event)
{
	wxTreeItemId id = event.GetItem();
	wxString text = GetItemText(id);
	int node_id = m_db.QueryByPath(m_db.GetDirPath() + "\\" + text.ToStdString());
	m_main_list->OnSelected(node_id);
}

void SearchList::GetSameImages(const std::vector<std::string>& src, float val, 
							   std::vector<std::vector<int> >& dst)
{
	if (src.size() <= 1) {
		return;
	}
	for (int i = 0; i < src.size() - 1; ++i) {
		std::vector<int> same;
		same.push_back(i);
		for (int j = i + 1; j < src.size(); ++j) {
			if (eimage::ImageCmp::IsSame(src[i], src[j], val)) {
				same.push_back(j);
			}
		}
		if (same.size() > 1) {
			dst.push_back(same);
		}
	}
}

}