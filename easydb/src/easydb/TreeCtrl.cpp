#include "TreeCtrl.h"
#include "Context.h"
#include "StagePanel.h"
#include "IndexNode.h"
#include "LeafNode.h"
#include "Database.h"
#include "DBHelper.h"

#include <ee/FileHelper.h>
#include <ee/SpriteSelection.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolType.h>

#include <easycomplex.h>

#include <sprite2/SymType.h>
#include <gum/StringHelper.h>
#include <gum/FilepathHelper.h>

#include <wx/clipbrd.h>

namespace edb
{

BEGIN_EVENT_TABLE(TreeCtrl, wxTreeCtrl)
	EVT_TREE_ITEM_MENU(ID_CTRL, TreeCtrl::OnItemMenu)
	EVT_TREE_ITEM_ACTIVATED(ID_CTRL, TreeCtrl::OnItemActivated)
	EVT_TREE_SEL_CHANGED(ID_CTRL, TreeCtrl::OnSelChanged)
	EVT_TREE_ITEM_ACTIVATED(ID_CTRL, TreeCtrl::OnItemClick)
	EVT_TREE_BEGIN_DRAG(ID_CTRL, TreeCtrl::OnBeginDrag)
	EVT_TREE_END_DRAG(ID_CTRL, TreeCtrl::OnEndDrag)
END_EVENT_TABLE()

static const wxColour COL_RED    = wxColour(204, 0, 0);
static const wxColour COL_GREEN  = wxColour(0, 204, 0);
static const wxColour COL_YELLOW = wxColour(204, 204, 0);

TreeCtrl::TreeCtrl(wxWindow* parent)
	: wxTreeCtrl(parent, ID_CTRL)
	, m_db(NULL)
{
}

void TreeCtrl::Build(const Database& db)
{
	m_db = &db;

	bool empty = IsEmpty();

	Clear();

	m_root = AddRoot(db.GetDirPath());

	BuildFromNode(db, db.GetRoot(), m_root, true);

	Expand(m_root);
}

void TreeCtrl::OnSelected(int node_id)
{
	std::map<int, wxTreeItemId>::iterator itr = m_map2id.find(node_id);
	if (itr != m_map2id.end()) {
		SelectItem(itr->second);
	}
}

void TreeCtrl::Clear()
{
	DeleteAllItems();
	m_map2node.clear();
	m_map2id.clear();
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
		SetItemStatus(curr, node);
	}
	m_map2node.insert(std::make_pair(curr, node->GetID()));
	m_map2id.insert(std::make_pair(node->GetID(), curr));

	if (const IndexNode* idx_node = dynamic_cast<const IndexNode*>(node)) {
		const std::vector<int>& children = idx_node->GetChildren();
		for (int i = 0, n = children.size(); i < n; ++i) {
			BuildFromNode(db, children[i], curr, false);
		}
	}
}

void TreeCtrl::SetItemStatus(wxTreeItemId id, const Node* node)
{
	if (node->Type() == NODE_INDEX) {
		return;
	}

	bool green = false, red = false;

	const LeafNode* leaf = dynamic_cast<const LeafNode*>(node);
	if (leaf->GetNodes(true).empty() && leaf->GetExportName().empty()) {
		green = true;
	}
	const std::set<int>& out = leaf->GetNodes(false);
	if (out.find(-1) != out.end()) {
		red = true;
	}
	if (green && red) {
		SetItemTextColour(id, COL_YELLOW);
	} else if (green) {
		SetItemTextColour(id, COL_GREEN);
	} else if (red) {
		SetItemTextColour(id, COL_RED);
	}
}

std::string TreeCtrl::GetItemName(const ee::Sprite& spr) const
{
	std::string name = dynamic_cast<const ee::Symbol*>(spr.GetSymbol())->GetFilepath();
	name = ee::FileHelper::GetFilename(name);
	return name;
}

void TreeCtrl::OnItemMenu(wxTreeEvent& event)
{
	wxTreeItemId id = event.GetItem();
	if (!id.IsOk()) {
		return;
	}	

	wxPoint clientpt = event.GetPoint();
	wxPoint screenpt = ClientToScreen(clientpt);
	ShowMenu(id, clientpt);
	event.Skip();
}

void TreeCtrl::OnItemActivated(wxTreeEvent& event)
{
	wxTreeItemId id = event.GetItem();
	if (id.IsOk()) {
		OpenFileByEditor(id);
	}
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
}

void TreeCtrl::OnEndDrag(wxTreeEvent& event)
{

}

void TreeCtrl::ShowMenu(wxTreeItemId id, const wxPoint& pt)
{
	m_on_menu_id = id;

	wxMenu menu;

	menu.Append(ID_MENU_OPEN, wxT("打开"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeCtrl::OnMenuOpen, this, ID_MENU_OPEN);

	menu.Append(ID_MENU_OPEN_PATH, wxT("打开路径"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeCtrl::OnMenuOpenPath, this, ID_MENU_OPEN_PATH);

	menu.Append(ID_MENU_COPY_FILENAME, wxT("复制文件名"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeCtrl::OnMenuCopyFilename, this, ID_MENU_COPY_FILENAME);

	menu.Append(ID_MENU_COPY_FILEPATH, wxT("复制完整路径和文件名"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeCtrl::OnMenuCopyFilepath, this, ID_MENU_COPY_FILEPATH);

	menu.AppendSeparator();

	menu.Append(ID_MENU_REF_INFO, wxT("引用信息"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeCtrl::OnMenuRefInfo, this, ID_MENU_REF_INFO);

	menu.Append(ID_MENU_COPY_TREE_TO, wxT("拷贝整棵树..."));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeCtrl::OnMenuCopyTreeTo, this, ID_MENU_COPY_TREE_TO);

	menu.Append(ID_MENU_DEL_TREE, wxT("删除整棵树"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeCtrl::OnMenuDelTree, this, ID_MENU_DEL_TREE);

	PopupMenu(&menu, pt);
}

void TreeCtrl::OnMenuOpen(wxCommandEvent& event)
{
	if (m_on_menu_id.IsOk()) {
		OpenFileByEditor(m_on_menu_id);
	}
}

void TreeCtrl::OnMenuOpenPath(wxCommandEvent& event)
{
	if (!m_on_menu_id.IsOk()) {
		return;
	}

	std::map<wxTreeItemId, int>::iterator itr = m_map2node.find(m_on_menu_id);
	const Node* node = m_db->Fetch(itr->second);
	std::string path = m_db->GetDirPath() + "\\" + node->GetPath();
	
	std::string command = "explorer /select, " + path;
	wxExecute(command, wxEXEC_ASYNC, NULL);
}

void TreeCtrl::OnMenuCopyFilename(wxCommandEvent& event)
{
	if (!m_on_menu_id.IsOk()) {
		return;
	}

	std::map<wxTreeItemId, int>::iterator itr = m_map2node.find(m_on_menu_id);
	const Node* node = m_db->Fetch(itr->second);
	std::string name = gum::FilepathHelper::Filename(node->GetPath());
	if (wxTheClipboard->Open()) {
		wxTheClipboard->SetData(new wxTextDataObject(name.c_str()));
		wxTheClipboard->Close();
	}
}

void TreeCtrl::OnMenuCopyFilepath(wxCommandEvent& event)
{
	if (!m_on_menu_id.IsOk()) {
		return;
	}

	std::map<wxTreeItemId, int>::iterator itr = m_map2node.find(m_on_menu_id);
	const Node* node = m_db->Fetch(itr->second);
	std::string path = m_db->GetDirPath() + "\\" + node->GetPath();
	if (wxTheClipboard->Open()) {
		wxTheClipboard->SetData(new wxTextDataObject(path.c_str()));
		wxTheClipboard->Close();
	}
}

static std::string NodesToStr(const Database& db, const std::set<int>& nodes)
{
	std::string str;
	std::set<int>::const_iterator itr = nodes.begin();
	for ( ; itr != nodes.end(); ++itr)
	{
		int id = *itr;
		if (id == -1) {
			continue;;
		}
		const Node* node = db.Fetch(id);
		if (node->Type() == NODE_INDEX) {
			continue;;
		}
		str += node->GetPath() + "\n";
	}
	return str;
}

void TreeCtrl::OnMenuRefInfo(wxCommandEvent& event)
{
	if (!m_on_menu_id.IsOk()) {
		return;
	}

	std::map<wxTreeItemId, int>::iterator itr = m_map2node.find(m_on_menu_id);
	const Node* node = m_db->Fetch(itr->second);
	if (node->Type() == NODE_INDEX) {
		return;
	}

	const LeafNode* leaf = static_cast<const LeafNode*>(node);

	bool closure = DBHelper::IsTreeClosure(*m_db, leaf);

	const std::set<int>& in_nodes = leaf->GetNodes(true);
	const std::set<int>& out_nodes = leaf->GetNodes(false);

	std::string in_str = NodesToStr(*m_db, in_nodes);
	std::string out_str = NodesToStr(*m_db, out_nodes);

	wxString msg;
	msg.Printf("被引用数量: %d\n%s\n引用数量: %d\n%s\n闭包: %s", 
		in_nodes.size(), 
		in_str,
		out_nodes.size(),
		out_str,
		closure ? "yes" : "no");
	wxMessageBox(msg, "ref info");
}

void TreeCtrl::OnMenuCopyTreeTo(wxCommandEvent& event)
{
	if (!m_on_menu_id.IsOk()) {
		return;
	}

	std::map<wxTreeItemId, int>::iterator itr = m_map2node.find(m_on_menu_id);
	const Node* node = m_db->Fetch(itr->second);
	if (node->Type() == NODE_INDEX) {
		return;
	}

	wxDirDialog dlg(NULL, "目标文件夹", wxEmptyString,
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK) {
		const LeafNode* leaf = static_cast<const LeafNode*>(node);
		DBHelper::CopyTree(*m_db, leaf, dlg.GetPath().ToStdString());
	}
}

void TreeCtrl::OnMenuDelTree(wxCommandEvent& event)
{
	if (!m_on_menu_id.IsOk()) {
		return;
	}

	std::map<wxTreeItemId, int>::iterator itr = m_map2node.find(m_on_menu_id);
	const Node* node = m_db->Fetch(itr->second);
	if (node->Type() == NODE_INDEX) {
		return;
	}

	const LeafNode* leaf = static_cast<const LeafNode*>(node);

	bool closure = DBHelper::IsTreeClosure(*m_db, leaf);
	bool in = !leaf->GetNodes(true).empty();
	if (!closure || in)
	{
		std::string msg;
		if (!closure && in) {
			msg = "不是闭包并且被别的节点引用";
		} else if (!closure) {
			msg = "不是闭包";
		} else if (in) {
			msg = "被别的节点引用";
		}
		int answer = wxMessageBox(msg, "确定要删除吗？", wxYES_NO | wxCANCEL, this);
		if (answer == wxYES) {
			DBHelper::DeleteTree(*m_db, node->GetID());
		}
	} 
	else
	{
		DBHelper::DeleteTree(*m_db, node->GetID());		
	}
}

void TreeCtrl::OpenFileByEditor(wxTreeItemId id)
{
	std::map<wxTreeItemId, int>::iterator itr = m_map2node.find(id);
	const Node* node = m_db->Fetch(itr->second);
	std::string path = m_db->GetDirPath() + "\\" + node->GetPath();

	int type = ee::SymbolFile::Instance()->Type(path);
	switch (type)
	{
	case s2::SYM_COMPLEX:
		wxExecute("easycomplex.exe " + path, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_ANIMATION:
		wxExecute("easyanimation.exe " + path, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_ICON:
		wxExecute("easyicon.exe " + path, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_MASK:
		wxExecute("easymask.exe " + path, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_MESH:
		wxExecute("easymesh.exe " + path, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_PARTICLE3D:
		wxExecute("easyparticle3d.exe " + path, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_PARTICLE2D:
		wxExecute("easyparticle2d.exe " + path, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_SCALE9:
		wxExecute("easyscale9.exe " + path, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_TEXTURE:
		wxExecute("easytexture.exe " + path, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_TRAIL:
		wxExecute("easytrail.exe " + path, wxEXEC_ASYNC, NULL);
		break;
	case ee::SYM_UI: case ee::SYM_UIWND:
		wxExecute("easyui.exe " + path, wxEXEC_ASYNC, NULL);
		break;
	}
}

}