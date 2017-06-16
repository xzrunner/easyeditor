#include "TreeCtrl.h"
//#include "Graphics.h"
#include "Context.h"
#include "StagePanel.h"
#include "IndexNode.h"
#include "Database.h"

#include <ee/FileHelper.h>
#include <ee/SpriteSelection.h>
#include <ee/SymbolFile.h>

#include <easycomplex.h>

#include <sprite2/SymType.h>
#include <gum/StringHelper.h>

namespace edb
{

BEGIN_EVENT_TABLE(TreeCtrl, wxTreeCtrl)
	EVT_TREE_SEL_CHANGED(ID_CTRL, TreeCtrl::OnSelChanged)
	EVT_TREE_ITEM_ACTIVATED(ID_CTRL, TreeCtrl::OnItemClick)
	EVT_TREE_BEGIN_DRAG(ID_CTRL, TreeCtrl::OnBeginDrag)
	EVT_TREE_END_DRAG(ID_CTRL, TreeCtrl::OnEndDrag)
END_EVENT_TABLE()

TreeCtrl::TreeCtrl(wxWindow* parent)
	: wxTreeCtrl(parent, ID_CTRL)
{
}

//void TreeCtrl::init(const Graphics& graph)
//{
//	bool empty = IsEmpty();
//
//	clear();
//	
//	if (empty)
//		m_root = AddRoot("Root");
//
//	std::map<ee::Sprite*, Node*>::const_iterator itr
//		= graph.connection.begin();
//	for ( ; itr != graph.connection.end(); ++itr)
//	{
//		wxTreeItemId id = AppendItem(m_root, GetItemName(*itr->first));
//		m_mapID2Sprite.insert(std::make_pair(id, itr->first));
//		addNode(graph, *itr->second, id);
//	}
//}

void TreeCtrl::Build(const Database& db)
{
	bool empty = IsEmpty();

	Clear();

//	if (empty) {
		m_root = AddRoot(db.GetDirPath());
//	}

	BuildFromNode(db, db.GetRoot(), m_root, true);
}

//void TreeCtrl::init(const wxArrayString& files)
//{
//	bool empty = IsEmpty();
//
//	clear();
//
//	if (empty)
//		m_root = AddRoot("Root");
//
//	for (size_t i = 0, n = files.size(); i < n; ++i)
//	{
//		std::string filepath = files[i];
//		int type = ee::SymbolFile::Instance()->Type(filepath);
//		if (type == s2::SYM_COMPLEX || type == s2::SYM_ANIMATION) {
//			std::string name = ee::FileHelper::GetFilename(filepath);
//			wxTreeItemId id = AppendItem(m_root, name);
//			m_mapID2Path.insert(std::make_pair(id, filepath));
//		}
//	}
//}

void TreeCtrl::Clear()
{
	DeleteAllItems();
	m_map2node.clear();
}

void TreeCtrl::BuildFromNode(const Database& db, int node_id, wxTreeItemId parent, bool first)
{
	const std::vector<Node*>& nodes = db.GetNodes();
	if (node_id < 0 || node_id >= nodes.size()) {
		return;
	}

	const Node* node = nodes[node_id];
	wxTreeItemId curr;
	if (first) {
		curr = m_root;
	} else {
		curr = AppendItem(parent, node->GetPath());
	}
	m_map2node.insert(std::make_pair(curr, node->GetID()));

	if (const IndexNode* idx_node = dynamic_cast<const IndexNode*>(node)) {
		const std::vector<int>& children = idx_node->GetChildren();
		for (int i = 0, n = children.size(); i < n; ++i) {
			BuildFromNode(db, children[i], curr, false);
		}
	}
}

//void TreeCtrl::addNode(const Graphics& graph, const Node& node, wxTreeItemId parent)
//{
//	for (size_t i = 0, n = node.out.size(); i < n; ++i)
//	{
//		ee::Sprite* child = node.out[i];
//		wxTreeItemId id = AppendItem(parent, GetItemName(*child));
//		m_mapID2Sprite.insert(std::make_pair(id, child));
//
//		Node* query = graph.Query(child);
//		if (query)
//			addNode(graph, *query, id);
//	}
//}

std::string TreeCtrl::GetItemName(const ee::Sprite& spr) const
{
	std::string name = dynamic_cast<const ee::Symbol*>(spr.GetSymbol())->GetFilepath();
	name = ee::FileHelper::GetFilename(name);
	return name;
}

void TreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	//ee::Sprite* spr = querySpriteByID(event.GetItem());
	//if (!spr) return;
	//
	//ee::SpriteSelection* selection = Context::Instance()->stage->GetSpriteSelection();
	//selection->Clear();
	//selection->Add(spr);

	//m_parent->Refresh(true);
}

void TreeCtrl::OnItemClick(wxTreeEvent& event)
{
//	ee::Sprite* spr = querySpriteByID(event.GetItem());
//	if (spr) 
//	{
//		if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(spr))
//		{
//			StagePanel* stage = Context::Instance()->stage;
//
//			ecomplex::Symbol* sym = dynamic_cast<ecomplex::Symbol*>(complex->GetSymbol());
//			ecomplex::EditDialog dlg(stage, sym, NULL);
//			dlg.ShowModal();
//		}
//	}
//	else
//	{
//		std::map<wxTreeItemId, std::string>::iterator itr = m_mapID2Path.find(event.GetItem());
//		if (itr != m_mapID2Path.end())
//		{
//			std::string filename = itr->second;
//			int type = ee::SymbolFile::Instance()->Type(filename);
//			switch (type)
//			{
//			case s2::SYM_COMPLEX:
//				{
//					std::string cmd = "easycomplex.exe " + itr->second;
//					WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
//				}
//				break;
//			case s2::SYM_ANIMATION:
//				{
//					std::string cmd = "easyanimation.exe " + itr->second;
//					WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
//				}
//				break;
//			}
//
//// 				ee::Symbol* sym = ee::SymbolMgr::Instance()->getSymbol(itr->second);
//// 				if (ecomplex::Symbol* complex = dynamic_cast<ecomplex::Symbol*>(sym))
//// 				{
////  					StagePanel* stage = Context::Instance()->stage;
////  					ecomplex::EditDialog dlg(stage, complex);
////  					dlg.ShowModal();
////  					stage->resetCanvas();
//// 				}
//// 				else if (libanim::Symbol* anim = dynamic_cast<libanim::Symbol*>(sym))
//// 				{
//// 
//// 				}
//		}
//	}
}

void TreeCtrl::OnBeginDrag(wxTreeEvent& event)
{
	wxTreeItemId id = event.GetItem();
	std::map<wxTreeItemId, int>::iterator itr = m_map2node.find(id);
	if (itr == m_map2node.end()) {
		return;
	}

	wxTextDataObject tdo(gum::StringHelper::ToString(itr->second));
	wxDropSource ds(tdo);
	ds.DoDragDrop(wxDrag_CopyOnly);

//	event.Allow();
}

void TreeCtrl::OnEndDrag(wxTreeEvent& event)
{

}

}