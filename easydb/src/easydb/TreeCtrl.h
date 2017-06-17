#ifndef _EASYDB_TREE_CTRL_H_
#define _EASYDB_TREE_CTRL_H_

#include <wx/treectrl.h>

#include <map>

namespace ee { class Sprite; }

namespace edb
{

class Database;
class Node;

class TreeCtrl : public wxTreeCtrl
{
public:
	TreeCtrl(wxWindow* parent);

	void Build(const Database& db);

	void OnSelected(int node_id);

	void Clear();

private:
	enum
	{
		ID_MENU_OPEN = 100,
		ID_MENU_OPEN_PATH,
		ID_MENU_COPY_FILENAME,
		ID_MENU_COPY_FILEPATH,
		ID_MENU_REF_INFO,
		ID_MENU_COPY_TREE_TO,
		ID_MENU_DEL_TREE,

		ID_CTRL = 1000,
	};

private:
	void BuildFromNode(const Database& db, int node, wxTreeItemId parent, bool first);

	void SetItemStatus(wxTreeItemId id, const Node* node);

	std::string GetItemName(const ee::Sprite& spr) const;

	void OnItemMenu(wxTreeEvent& event);
	void OnItemActivated(wxTreeEvent& event);
	void OnBeginDrag(wxTreeEvent& event);
	void OnEndDrag(wxTreeEvent& event);

	void ShowMenu(wxTreeItemId id, const wxPoint& pt);

	void OnMenuOpen(wxCommandEvent& event);
	void OnMenuOpenPath(wxCommandEvent& event);
	void OnMenuCopyFilename(wxCommandEvent& event);
	void OnMenuCopyFilepath(wxCommandEvent& event);
	void OnMenuRefInfo(wxCommandEvent& event);
	void OnMenuCopyTreeTo(wxCommandEvent& event);
	void OnMenuDelTree(wxCommandEvent& event);

	void OpenFileByEditor(wxTreeItemId id);

private:
	const Database* m_db;

	wxTreeItemId m_root;

	std::map<wxTreeItemId, int> m_map2node;
	std::map<int, wxTreeItemId> m_map2id;

	wxTreeItemId m_on_menu_id;

	DECLARE_EVENT_TABLE()

}; // TreeCtrl

}

#endif // _EASYDB_TREE_CTRL_H_s