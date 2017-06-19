#include "MainList.h"
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

BEGIN_EVENT_TABLE(MainList, wxTreeCtrl)
	EVT_TREE_ITEM_MENU(ID_CTRL, MainList::OnItemMenu)
	EVT_TREE_ITEM_ACTIVATED(ID_CTRL, MainList::OnItemActivated)
	EVT_TREE_BEGIN_DRAG(ID_CTRL, MainList::OnBeginDrag)
	EVT_TREE_END_DRAG(ID_CTRL, MainList::OnEndDrag)
END_EVENT_TABLE()

static const wxColour COL_RED    = wxColour(204, 0, 0);
static const wxColour COL_GREEN  = wxColour(0, 204, 0);
static const wxColour COL_YELLOW = wxColour(204, 204, 0);

MainList::MainList(wxWindow* parent)
	: wxTreeCtrl(parent, ID_CTRL)
	, m_db(NULL)
{
	SetBackgroundColour(wxColour(229, 229, 229));
}

void MainList::Build(const Database& db)
{
	m_db = &db;

	bool empty = IsEmpty();

	Clear();

	m_root = AddRoot(db.GetDirPath());

	BuildFromNode(db, db.GetRoot(), m_root, true);

	Expand(m_root);
}

void MainList::OnSelected(int node_id)
{
	std::map<int, wxTreeItemId>::iterator itr = m_map2id.find(node_id);
	if (itr != m_map2id.end()) {
		SelectItem(itr->second);
	}
}

void MainList::Clear()
{
	DeleteAllItems();
	m_map2node.clear();
	m_map2id.clear();
}

void MainList::BuildFromNode(const Database& db, int node_id, wxTreeItemId parent, bool first)
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

	if (node->Type() == NODE_INDEX) {
		const std::vector<int>& children = static_cast<const IndexNode*>(node)->GetChildren();
		for (int i = 0, n = children.size(); i < n; ++i) {
			BuildFromNode(db, children[i], curr, false);
		}
	}
}

void MainList::SetItemStatus(wxTreeItemId id, const Node* node)
{
	if (node->Type() == NODE_INDEX) {
		return;
	}

	const LeafNode* leaf = static_cast<const LeafNode*>(node);
	bool green = leaf->IsNoUse(), 
		 red = leaf->IsRefError();
	if (green && red) {
		SetItemTextColour(id, COL_YELLOW);
	} else if (green) {
		SetItemTextColour(id, COL_GREEN);
	} else if (red) {
		SetItemTextColour(id, COL_RED);
	}
}

void MainList::OnItemMenu(wxTreeEvent& event)
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

void MainList::OnItemActivated(wxTreeEvent& event)
{
	wxTreeItemId id = event.GetItem();
	if (id.IsOk()) {
		OpenFileByEditor(id);
	}
}

void MainList::OnBeginDrag(wxTreeEvent& event)
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

void MainList::OnEndDrag(wxTreeEvent& event)
{

}

void MainList::ShowMenu(wxTreeItemId id, const wxPoint& pt)
{
	m_on_menu_id = id;

	wxMenu menu;

	menu.Append(ID_MENU_OPEN, wxT("打开"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainList::OnMenuOpen, this, ID_MENU_OPEN);

	menu.Append(ID_MENU_OPEN_PATH, wxT("打开路径"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainList::OnMenuOpenPath, this, ID_MENU_OPEN_PATH);

	menu.Append(ID_MENU_COPY_FILENAME, wxT("复制文件名"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainList::OnMenuCopyFilename, this, ID_MENU_COPY_FILENAME);

	menu.Append(ID_MENU_COPY_FILEPATH, wxT("复制完整路径和文件名"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainList::OnMenuCopyFilepath, this, ID_MENU_COPY_FILEPATH);

	menu.AppendSeparator();

	menu.Append(ID_MENU_REF_INFO, wxT("引用信息"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainList::OnMenuRefInfo, this, ID_MENU_REF_INFO);

	menu.Append(ID_MENU_COPY_TREE_TO, wxT("拷贝整棵树..."));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainList::OnMenuCopyTreeTo, this, ID_MENU_COPY_TREE_TO);

	menu.Append(ID_MENU_DEL_TREE, wxT("删除整棵树"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainList::OnMenuDelTree, this, ID_MENU_DEL_TREE);

	menu.Append(ID_MENU_RENAME, wxT("重命名"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainList::OnMenuRename, this, ID_MENU_RENAME);

	menu.AppendSeparator();

	menu.Append(ID_MENU_BASE_INFO, wxT("基本信息"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainList::OnMenuBaseInfo, this, ID_MENU_BASE_INFO);

	PopupMenu(&menu, pt);
}

void MainList::OnMenuOpen(wxCommandEvent& event)
{
	if (m_on_menu_id.IsOk()) {
		OpenFileByEditor(m_on_menu_id);
	}
}

void MainList::OnMenuOpenPath(wxCommandEvent& event)
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

void MainList::OnMenuCopyFilename(wxCommandEvent& event)
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

void MainList::OnMenuCopyFilepath(wxCommandEvent& event)
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

void MainList::OnMenuRefInfo(wxCommandEvent& event)
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

void MainList::OnMenuCopyTreeTo(wxCommandEvent& event)
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

void MainList::OnMenuDelTree(wxCommandEvent& event)
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
		if (answer == wxYES) 
		{
			Delete(m_on_menu_id);
			DBHelper::DeleteTree(*m_db, node->GetID());
			if (in) {
				DBHelper::FixNodeRef(*m_db, static_cast<const LeafNode*>(node));
			}
		}
	}
	else
	{
		DBHelper::DeleteTree(*m_db, node->GetID());
	}

	m_on_menu_id.Unset();
}

void MainList::OnMenuRename(wxCommandEvent& event)
{
	if (!m_on_menu_id.IsOk()) {
		return;
	}

	std::map<wxTreeItemId, int>::iterator itr = m_map2node.find(m_on_menu_id);
	const Node* node = m_db->Fetch(itr->second);
	if (node->Type() == NODE_INDEX) {
		return;
	}

	std::string filename = m_db->GetDirPath() + "\\" + node->GetPath();
	filename = gum::FilepathHelper::Filename(filename);
	wxTextEntryDialog dlg(this, "Rename", wxGetTextFromUserPromptStr, filename);
	if (dlg.ShowModal()) 
	{
		std::string new_name = dlg.GetValue().ToStdString();
		if (new_name != filename) 
		{
			DBHelper::RenameNode(*m_db, static_cast<const LeafNode*>(node), new_name);
			std::string new_path = node->GetPath().substr(0, node->GetPath().find(filename));
			SetItemText(m_on_menu_id, new_path + new_name);
		}
	}
}

void MainList::OnMenuBaseInfo(wxCommandEvent& event)
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

	wxString msg;
	msg.Printf("修改时间: %d\nMD5: %s\n", 
		static_cast<uint32_t>(leaf->GetTimestamp()),
		leaf->GetMD5().c_str());
	wxMessageBox(msg, "base info");
}

void MainList::OpenFileByEditor(wxTreeItemId id)
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